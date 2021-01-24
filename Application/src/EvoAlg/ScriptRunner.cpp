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

#define EAS_SRM (m_GameRunner.GetSave().GetRobotsManagement())
#define EAS_SVS (m_GameRunner.GetSave().GetVillageStats())
namespace EvoAlg
{
    ScriptRunner::ScriptRunner(Script &script, GameRunner &gameRunner, const Individual &individual)
        : m_Script(script), m_GameRunner(gameRunner), m_Individual(individual), m_OperationFunctions(gameRunner)
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

        uint32_t currentTickDelay = m_GameRunner.GetGameConsts().GetTickDelay();

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

                //If it is a wait operation, hijack another parameter with DELAY_MICRO
                if (m_GameScript.at(curGameplay).at(curOp).at(0) == "6")
                    m_GameScript.at(curGameplay).at(curOp).push_back(std::to_string(currentTickDelay));
            }
        }
    }

    std::vector<TimeResult> *ScriptRunner::RunAllGameplays()
    {

        auto *gameplaysResults = new std::vector<TimeResult>();
        uint64_t gameplayCount = m_GameScript.size();

        //TODO?: async calling more gameplays (respecting MAX_THREADS) [problem with unique m_GameRunner]
        for (uint64_t curGameplay = 0; curGameplay < gameplayCount; curGameplay++)
        {
            gameplaysResults->push_back(RunGameplay(curGameplay));
            DE_TRACE("(ScriptRunner -- {0}) Changing gameplay ", m_Individual.ID);
        }

        return gameplaysResults;
    }

    TimeResult ScriptRunner::RunGameplay(uint64_t gameplayIndex)
    {
        //Reference Tickdelay as copy, in case of
        //TODO: assure that Tickdelay WON'T BE CHANGED in the middle of the gameplay (or else fitness will be wrong)
        uint32_t currentTickDelay = m_GameRunner.GetGameConsts().GetTickDelay();

        //TODO: change to bigger size (uint64/32)
        int measuredDurationInTicks = -1;

        //Create promises to synchronize this thread with game callbacks
        std::promise<void> gameStartedPromise, gameEndedPromise;
        std::future<void> gameEndedFuture = gameEndedPromise.get_future();
        std::future<void> gameStartedFuture = gameStartedPromise.get_future();

        //Defining game callbacks that will activate the promises above
        auto gameStartedConfirmationCB = [this, &gameStartedPromise](GameRunner &gameRunner) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameStarted", m_Individual.ID);
            gameStartedPromise.set_value();
            return false;
        };

        auto gameEndedConfirmationCB = [this, &measuredDurationInTicks, &gameEndedPromise](GameRunner &gameRunner, int elapsedTimeInTicks) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameEnded", m_Individual.ID);
            measuredDurationInTicks = elapsedTimeInTicks;
            gameEndedPromise.set_value();
            return false;
        };

        EH_GameStarted *gameStartedEventHandler = new EH_GameStarted(gameStartedConfirmationCB);
        EH_GameEnded *gameEndedEventHandler = new EH_GameEnded(gameEndedConfirmationCB);

        //Add to the GameRunner our callbacks
        m_GameRunner.RegisterOnGameStarted(gameStartedEventHandler);
        m_GameRunner.RegisterOnGameEnded(gameEndedEventHandler);

        //Request the game to start and wait it to be completely started
        DE_TRACE("(ScriptRunner -- {0}) Requesting gameplay #{1} to start", m_Individual.ID, gameplayIndex);
        {
            m_GameRunner.Start();
            gameStartedFuture.get();
        }
        DE_TRACE("(ScriptRunner -- {0}) Gameplay #{1} to started successfully", m_Individual.ID, gameplayIndex);

        //
        DE_TRACE("(ScriptRunner -- {0}) Starting operations of gameplay #{1}...", m_Individual.ID, gameplayIndex);
        auto &operations = m_GameScript.at(gameplayIndex);
        for (size_t i = 1; i < operations.size(); i++)
        {
            DE_DEBUG("(ScriptRunner -- {0}) Executing operation #{2} of gameplay #{1}", m_Individual.ID, gameplayIndex, i);

            m_OperationFunctions.Execute(operations[i]);

            //TODO: check if this is correct
            usleep(currentTickDelay);
        }
        DE_TRACE("(ScriptRunner -- {0}) All operations of gameplay #{1} executed Successfully!", m_Individual.ID, gameplayIndex);

        //TODO: remove test
        m_GameRunner.Stop();

        DE_TRACE("(ScriptRunner -- {0}) Waiting for gameplay #{1} to end...", m_Individual.ID, gameplayIndex);
        {
            //Waits for game to end
            gameEndedFuture.get();

            if (!m_GameRunner.IsGameLost())
            {
                DE_CRITICAL("(ScriptRunner -- {0}) After EH_GameEnded (gameplay #{1}), the game is not lost yet, waiting 1sec...", m_Individual.ID, gameplayIndex);
            }
        }
        DE_INFO("(ScriptRunner -- {0}) Gameplay #{1} ended normally", m_Individual.ID, gameplayIndex);

        DE_ASSERT(measuredDurationInTicks >= 0, "Game duration not calculated correctly");
        DE_ASSERT(m_GameRunner.IsGameLost(), "** Game ended but gameplay is not lost!!! **");

        double targetDurationAU = stol(m_GameScript.at(gameplayIndex).at(0).at(0));
        double measuredDurationAU = measuredDurationInTicks * AU_PER_TICK;

        m_GameRunner.UnregisterOnGameStarted(gameStartedEventHandler);
        m_GameRunner.UnregisterOnGameEnded(gameEndedEventHandler);

        return TimeResult{targetDurationAU, measuredDurationAU};
    }

#pragma region ScriptFunctions

    void ScriptFunctions::scriptFunct0(const std::vector<std::string> &params)
    {
        EAS_SRM->createTask(static_cast<RobotFunction>(stoi(params.at(1))));
    }

    void ScriptFunctions::scriptFunct1(const std::vector<std::string> &params)
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

    void ScriptFunctions::scriptFunct2(const std::vector<std::string> &params)
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

    void ScriptFunctions::scriptFunct3(const std::vector<std::string> &params)
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

    void ScriptFunctions::scriptFunct4(const std::vector<std::string> &params)
    {
        EAS_SRM->createRobots(1);
    }

    void ScriptFunctions::scriptFunct5(const std::vector<std::string> &params)
    {
        EAS_SRM->destroyRobots(1);
    }

    void ScriptFunctions::scriptFunct6(const std::vector<std::string> &params)
    {
        double waitsPerSecond = 1e6 / HUMAN_OP_DELAY;

        int waitTime = std::stoi(params.at(1));

        int waitCount = waitTime * waitsPerSecond;
        if (waitCount > 0)
            waitCount--;

        float DELAY_MICRO = std::stof(params.at(2));

        usleep(waitCount * DELAY_MICRO);
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