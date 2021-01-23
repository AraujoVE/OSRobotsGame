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

    static void *runScript(void *scriptFuncObject)
    {
        ScriptRunner *script = (ScriptRunner *)scriptFuncObject;
        return script->scriptLoop();
    }

    ScriptRunner::ScriptRunner(Script& script, GameRunner &gameRunner, const Individual& individual) 
        : m_Script(script), m_GameRunner(gameRunner), m_Individual(individual)
    {
        initScriptDirections();
    }

    void ScriptRunner::startScript()
    {
        pthread_create(&scriptThread, NULL, runScript, this);
    }

    ScriptRunner::~ScriptRunner()
    {
        pthread_join(scriptThread, NULL);
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
                gameScript.push_back(std::vector<std::vector<std::string>>());
                curGameplay++;
                curOp = 0;
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                gameScript.at(curGameplay).at(curOp).push_back(line.substr(1, line.length() - 1));
            }
            else
            {
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                curOp++;
                tokenPos = line.find(",");
                while (tokenPos != -1)
                {
                    gameScript.at(curGameplay).at(curOp).push_back(line.substr(0, tokenPos));
                    line = line.substr(tokenPos + 1, line.length() - (1 + tokenPos));
                    tokenPos = line.find(",");
                }
                gameScript.at(curGameplay).at(curOp).push_back(line);

                //If it is a wait operation, hijack another parameter with DELAY_MICRO
                if (gameScript.at(curGameplay).at(curOp).at(0) == "6")
                    gameScript.at(curGameplay).at(curOp).push_back(std::to_string(currentTickDelay));
            }
        }
    }

    void ScriptRunner::scriptFunct0(const std::vector<std::string> &params)
    {
        EAS_SRM->createTask(static_cast<RobotFunction>(stoi(params.at(1))));
    }

    void ScriptRunner::scriptFunct1(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid cancelTask #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptRunner:: scriptFunct1) Invalid Task " + params.at(2));
        EAS_SRM->cancelTask(*curTask.value());
    }

    void ScriptRunner::scriptFunct2(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot++ @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptRunner::scriptFunct2) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), 1);
    }

    void ScriptRunner::scriptFunct3(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot-- @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(ScriptRunner::scriptFunct3) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), -1);
    }

    void ScriptRunner::scriptFunct4(const std::vector<std::string> &params)
    {
        EAS_SRM->createRobots(1);
    }

    void ScriptRunner::scriptFunct5(const std::vector<std::string> &params)
    {
        EAS_SRM->destroyRobots(1);
    }

    void ScriptRunner::scriptFunct6(const std::vector<std::string> &params)
    {
        double waitsPerSecond = 1e6 / HUMAN_OP_DELAY;

        int waitTime = std::stoi(params.at(1));

        int waitCount = waitTime * waitsPerSecond;
        if (waitCount > 0)
            waitCount--;

        float DELAY_MICRO = std::stof(params.at(2));

        usleep(waitCount * DELAY_MICRO);
    }

    //TODO: Jogo versão normal ou calibração
    //TODO: Velocidade dos segundos do jogo -> slider

    std::vector<TimeResult> *ScriptRunner::scriptLoop()
    {
        int measuredDurationInTicks = -1;
        int it = 0;

        auto *gameplaysResults = new std::vector<TimeResult>();

        Application::Semaphore endSem;

        auto &_gameScript = gameScript;

        m_GameRunner.SetOnGameStarted(new EH_GameStarted([this, &_gameScript, &it](GameRunner &gameRunner) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameStarted", m_Individual.ID);

            uint32_t currentTickDelay = gameRunner.GetGameConsts().GetTickDelay();

            for (int i = 1; i < (int)_gameScript.at(it).size(); i++)
            {
                (this->*(scriptLoopFuncts[stoi(_gameScript.at(it).at(i).at(0))]))(_gameScript.at(it).at(i));
                usleep(currentTickDelay);
            }

            return false;
        }));

        m_GameRunner.SetOnGameEnded(new EH_GameEnded([this, &measuredDurationInTicks, &endSem, &_gameScript, &it, gameplaysResults](GameRunner &gameRunner,int elapsedTimeInTicks) {
            DE_DEBUG("(ScriptRunner -- {0}) OnGameEnded", m_Individual.ID);

            measuredDurationInTicks = elapsedTimeInTicks;
            endSem.Post();

            return false;
        }));

        for (int m = 0; m < (int)gameScript.size(); m++)
        {
            DE_INFO("(ScriptRunner -- {0}) Starting gameplay #{1}", m_Individual.ID, m);
            m_GameRunner.Start();

            DE_DEBUG("(ScriptRunner -- {0}) Waiting for gameplay #{1} to end...", m_Individual.ID, m);
            endSem.Wait();

            while (!m_GameRunner.IsGameLost()) {
                DE_ERROR("(ScriptRunner -- {0}) After EH_GameEnded (gameplay #{1}), the game is not lost yet, waiting 1sec..." ,m_Individual.ID, m);
                usleep(1e6); 
            }


            DE_INFO("(ScriptRunner -- {0}) Gameplay #{1} ended normally", m_Individual.ID, m);

            //Callback para saber quando acaba o jogo
            //quando isso acontece : endTime = time(0)
            double targetDurationAU = stol(_gameScript.at(it).at(0).at(0));
            double measuredDurationAU = measuredDurationInTicks * AU_PER_TICK;
            gameplaysResults->push_back(TimeResult{targetDurationAU, measuredDurationAU});
            it++;

            DE_ASSERT(measuredDurationInTicks >= 0, "Game duration not calculated correctly");
            DE_ASSERT(m_GameRunner.IsGameLost(), "** Game ended but it's not lost!!! **");

            DE_TRACE("(ScriptRunner -- {0}) Changing gameplay ", m_Individual.ID);
        }

        return gameplaysResults;
    }

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