#include "ScriptRunner.hpp"
#include "Application/ImGui/GameWindows/FunctionWindow.hpp"
#include "Application/ImGui/GameWindows/TaskWindow.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Application/Game/GameSave.hpp"

#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "Application/Game/Ingame/RobotsManagement.hpp"
#include "Application/Threads/Semaphore.hpp"

#include "mypch.hpp"

using namespace Application;

#define EAS_SRM (gameRunner.GetSave().GetRobotsManagement())
#define EAS_SVS (gameRunner.GetSave().GetVillageStats())
namespace EvoAlg
{
    ScriptRunner::ScriptRunner(Script &script)
        : m_Script(script)
    {
        initScriptDirections();
    }

    ScriptRunner::~ScriptRunner()
    {
    }

    void ScriptRunner::initScriptDirections()
    {
        int tokenPos, curGameplay = -1, curOp;
        std::string line;

        for (auto line : m_Script.m_Instructions)
        {
            if (line[0] == '#')
            {
                m_GameScript.push_back(std::vector<std::vector<std::string>>());
                curGameplay++;
                curOp = 0;
                m_GameScript.at(curGameplay).push_back(std::vector<std::string>());
                m_GameScript.at(curGameplay).at(curOp).push_back(line.substr(1, line.length() - 1));
            }
            else
            {
                m_GameScript.at(curGameplay).push_back(std::vector<std::string>());
                curOp++;
                tokenPos = line.find(",");
                while (tokenPos != -1)
                {
                    m_GameScript.at(curGameplay).at(curOp).push_back(line.substr(0, tokenPos));
                    line = line.substr(tokenPos + 1, line.length() - (1 + tokenPos));
                    tokenPos = line.find(",");
                }
                m_GameScript.at(curGameplay).at(curOp).push_back(line);
            }
        }
    }

    std::vector<TimeResult> *ScriptRunner::RunAllGameplays(GameRunner &gameRunner, Individual &individual)
    {

        auto *gameplaysResults = new std::vector<TimeResult>();
        uint64_t gameplayCount = m_GameScript.size();

        //TODO?: async calling more gameplays (respecting MAX_THREADS) [problem with unique m_GameRunner]
        for (uint64_t curGameplayIdx = 0; curGameplayIdx < gameplayCount; curGameplayIdx++)
        {
            gameplaysResults->push_back(RunGameplay(gameRunner, individual, curGameplayIdx));
            DE_TRACE("(ScriptRunner -- {0}) Changing gameplay ", individual.ID);
        }

        return gameplaysResults;
    }

    TimeResult ScriptRunner::RunGameplay(GameRunner &gameRunner, Individual &individual, uint64_t gameplayIndex)
    {
        //TODO: change to bigger size (uint64/32)
        int measuredDurationInTicks = -1;

        //Create promises to synchronize this thread with game callbacks
        std::promise<void> gameStartedPromise, gameEndedPromise;
        std::future<void> gameEndedFuture = gameEndedPromise.get_future();
        std::future<void> gameStartedFuture = gameStartedPromise.get_future();

        //Defining game callbacks that will activate the promises above
        auto gameStartedConfirmationCB = [&individual, &gameStartedPromise](GameRunner &gameRunner) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameStarted", individual.ID);
            gameStartedPromise.set_value();
            return false;
        };

        auto gameEndedConfirmationCB = [&individual, &measuredDurationInTicks, &gameEndedPromise](GameRunner &gameRunner, int elapsedTimeInTicks) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameEnded", individual.ID);
            measuredDurationInTicks = elapsedTimeInTicks;
            gameEndedPromise.set_value();
            return false;
        };

        EH_GameStarted *gameStartedEventHandler = new EH_GameStarted(gameStartedConfirmationCB);
        EH_GameEnded *gameEndedEventHandler = new EH_GameEnded(gameEndedConfirmationCB);

        //Add to the GameRunner our callbacks
        gameRunner.RegisterOnGameStarted(gameStartedEventHandler);
        gameRunner.RegisterOnGameEnded(gameEndedEventHandler);

        //Request the game to start and wait it to be completely started
        DE_TRACE("(ScriptRunner -- {0}) Requesting gameplay #{1} to start", individual.ID, gameplayIndex);
        {
            gameRunner.Start();
            gameStartedFuture.get();
        }
        DE_TRACE("(ScriptRunner -- {0}) Gameplay #{1} to started successfully", individual.ID, gameplayIndex);

        //
        DE_TRACE("(ScriptRunner -- {0}) Starting operations of gameplay #{1}...", individual.ID, gameplayIndex);
        auto &operations = m_GameScript.at(gameplayIndex);
        for (size_t i = 1; i < operations.size(); i++)
        {
            DE_DEBUG("(ScriptRunner -- {0}) Executing operation #{2} of gameplay #{1}", individual.ID, gameplayIndex, i);

            m_OperationFunctions.Execute(gameRunner, operations[i]);

            //TODO: check if this is correct
            usleep(gameRunner.GetGameConsts().GetTickDelay());
        }
        DE_TRACE("(ScriptRunner -- {0}) All operations of gameplay #{1} executed Successfully!", individual.ID, gameplayIndex);

        //Waits for game to end
        DE_TRACE("(ScriptRunner -- {0}) Waiting for gameplay #{1} to end...", individual.ID, gameplayIndex);
        gameEndedFuture.get();
        DE_INFO("(ScriptRunner -- {0}) Gameplay #{1} ended normally", individual.ID, gameplayIndex);

        DE_ASSERT(measuredDurationInTicks >= 0, "Game duration not calculated correctly");
        DE_ASSERT(gameRunner.IsGameLost(), "** Game ended but gameplay is not lost!!! **");

        double targetDurationAU = stol(m_GameScript.at(gameplayIndex).at(0).at(0));
        double measuredDurationAU = measuredDurationInTicks * AU_PER_TICK;

        gameRunner.UnregisterOnGameStarted(gameStartedEventHandler);
        gameRunner.UnregisterOnGameEnded(gameEndedEventHandler);

        return TimeResult{targetDurationAU, measuredDurationAU};
    }

#pragma region ScriptFunctions

    void ScriptFunctions::scriptFunct0(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        EAS_SRM->createTask(static_cast<RobotFunction>(stoi(params.at(1))));
    }

    void ScriptFunctions::scriptFunct1(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid cancelTask #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptFunctions:: scriptFunct1) Invalid Task " + params.at(2));
        EAS_SRM->cancelTask(*curTask.value());
    }

    void ScriptFunctions::scriptFunct2(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot++ @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptFunctions::scriptFunct2) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), 1);
    }

    void ScriptFunctions::scriptFunct3(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot-- @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptFunctions::scriptFunct3) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), -1);
    }

    void ScriptFunctions::scriptFunct4(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        EAS_SRM->createRobots(1);
    }

    void ScriptFunctions::scriptFunct5(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        EAS_SRM->destroyRobots(1);
    }

    void ScriptFunctions::scriptFunct6(GameRunner &gameRunner, const std::vector<std::string> &params)
    {
        double waitsPerSecond = 1e6 / HUMAN_OP_DELAY;

        int waitTime = std::stoi(params.at(1));

        int waitCount = waitTime * waitsPerSecond;
        if (waitCount > 0)
            waitCount--;

        while (waitCount-- > 0) {
            if (gameRunner.IsGameLost()) return;
            
            usleep(gameRunner.GetGameConsts().GetTickDelay());
        }
    }

    void ScriptFunctions::Execute(GameRunner &gameRunner, const std::vector<std::string> &operation)
    {
        (this->*(scriptLoopFuncts[stoi(operation.at(0))]))(gameRunner, operation);
    }

#pragma endregion

} // namespace EvoAlg

/*
task++(par1) //Cria uma nova Task
    //par1 : Tipo de Function da Task{
        pro,
        hun,
        med,
        con,
        res
    }

task--(par1,par2) //Termina uma Task existente
    //par1 : Função da Task
    //par2 : Numero da Task removida

move++(par1,par2) // Move os robos para uma Task
    //par1 : Função da Task afetada
    //par2 : Numero da Task afetada


move--(par1,par2) // Remove os robos de uma Task
    //par1 : Função da Task afetada
    //par2 : Numero da Task afetada


robots++ //Incrementa o número de robôs

robots-- //Destroi um robô

wait(par1) // Espera um certo tempo até a próxima ação
    //par1 : tempo de espera em WAIT_UNITS
*/

/*
//task++(med)
0,2

//task--(pro,5)
1,0,5

//move++(con,6)
2,3,6

//move--(med,9)
3,2,9

//robots++
4

//robots--
5

//wait(3)
6,3
*/

/*
task++(med)
task--(pro,5)
move++(con,6)
move--(med,9)
robots++
robots--
wait(3)
end(182)
*/

/*
task++(med)
task--(pro,5)
move++(con,6)
move--(med,9)
robots++
robots++
wait(10)
end(122)
*/