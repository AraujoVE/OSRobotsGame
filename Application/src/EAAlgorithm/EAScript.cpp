#include "EAScript.hpp"
#include "Application/ImGui/GameWindows/FunctionWindow.hpp"
#include "Application/ImGui/GameWindows/TaskWindow.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Application/Game/GameSave.hpp"

#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "Application/header/RobotsManagement.hpp"
#include "Application/Threads/Semaphore.hpp"

#include "mypch.hpp"

using namespace Application;

#define EAS_SRM (m_GameRunner.GetSave().GetRobotsManagement())
#define EAS_SVS (m_GameRunner.GetSave().GetVillageStats())
namespace EAAlgorithm
{

    static void *runScript(void *scriptFuncObject)
    {
        EAScript *script = (EAScript *)scriptFuncObject;
        return script->scriptLoop();
    }

    EAScript::EAScript(GameRunner &gameRunner, std::string filePath) : m_GameRunner(gameRunner),
                                                                       srcFile(filePath)
    {
        initScriptDirections();
    }

    void EAScript::startScript()
    {
        pthread_create(&scriptThread, NULL, runScript, this);
    }

    EAScript::~EAScript()
    {
        pthread_join(scriptThread, NULL);
    }

    void EAScript::initScriptDirections()
    {
        std::fstream myFile;
        int tokenPos, curGameplay = -1, curOp;
        myFile.open(srcFile, std::ios::in);
        std::string line;
        float DELAY_MICRO = m_GameRunner.GetGameConsts().GetValue("DECAY_DELAY_MICRO");

        while (std::getline(myFile, line))
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
                    gameScript.at(curGameplay).at(curOp).push_back(std::to_string(DELAY_MICRO));
            }
        }
    }

    void EAScript::scriptFunct0(const std::vector<std::string> &params)
    {
        EAS_SRM->createTask(static_cast<RobotFunction>(stoi(params.at(1))));
    }

    void EAScript::scriptFunct1(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid cancelTask #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(EAScript:: scriptFunct1) Invalid Task " + params.at(2));
        EAS_SRM->cancelTask(*curTask.value());
    }

    void EAScript::scriptFunct2(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot++ @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(EAScript::scriptFunct2) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), 1);
    }

    void EAScript::scriptFunct3(const std::vector<std::string> &params)
    {
        auto curTask = EAS_SRM->findTask(stoi(params.at(2)), (RobotFunction)stoi(params.at(1)));
        if (!curTask.has_value())
        {
            DE_WARN("Ignoring invalid moveRobot-- @Task #{0}", params.at(2));
            return;
        }
        DE_ASSERT(curTask.has_value(), "(EAScript::scriptFunct3) Invalid Task " + params.at(2));
        EAS_SRM->moveRobot(*curTask.value(), -1);
    }

    void EAScript::scriptFunct4(const std::vector<std::string> &params)
    {
        EAS_SRM->createRobots(1);
    }

    void EAScript::scriptFunct5(const std::vector<std::string> &params)
    {
        EAS_SRM->destroyRobots(1);
    }

    void EAScript::scriptFunct6(const std::vector<std::string> &params)
    {
        double waitsPerSecond = 1e6 / HUMAN_WAIT_UNIT;

        int waitTime = std::stoi(params.at(1));

        int waitCount = waitTime * waitsPerSecond;
        if (waitCount > 0)
            waitCount--;

        float DELAY_MICRO = std::stof(params.at(2));

        usleep(waitCount * DELAY_MICRO);
    }

    //TODO: Jogo versão normal ou calibração
    //TODO: Velocidade dos segundos do jogo -> slider

    std::vector<std::pair<double, double>> *EAScript::scriptLoop()
    {
        time_t initTime, endTime;
        int it = 0;

        auto *gameplaysResults = new std::vector<std::pair<double, double>>();

        float DELAY_MICRO = m_GameRunner.GetGameConsts().GetValue("DECAY_DELAY_MICRO");

        Application::Semaphore endSem;

        auto &_gameScript = gameScript;

        m_GameRunner.SetOnGameStarted(new EH_GameStarted([&initTime, this, &_gameScript, &it, DELAY_MICRO](GameRunner &_) {
            DE_DEBUG("(EAScript) Game Started");
            initTime = time(0);

            for (int i = 1; i < (int)_gameScript.at(it).size(); i++)
            {
                std::cout << "\tAction " << i << std::endl;
                (this->*(scriptLoopFuncts[stoi(_gameScript.at(it).at(i).at(0))]))(_gameScript.at(it).at(i));
                usleep(DELAY_MICRO);
            }

            return false;
        }));

        auto &gameRunner = m_GameRunner;

        m_GameRunner.SetOnGameEnded(new EH_GameEnded([&endTime, &endSem, &_gameScript, &it, &initTime, gameplaysResults, &gameRunner](GameRunner &_) {
            DE_DEBUG("(EAScript) Game Ended");
            endTime = time(0);

            //Callback para saber quando acaba o jogo
            //quando isso acontece : endTime = time(0)
            time_t targetDurationAU = stol(_gameScript.at(it).at(0).at(0));
            time_t realDurationS = endTime - initTime;

            double AUperS = HUMAN_WAIT_UNIT / gameRunner.GetGameConsts().GetValue("DECAY_DELAY_MICRO");
            time_t realDurationAU = realDurationS * AUperS;

            gameplaysResults->push_back({targetDurationAU, realDurationAU});

            DE_DEBUG("(EAScript) UNLOCK 1");
            endSem.Post();

            return false;
        }));

        for (int m = 0; m < (int)gameScript.size(); m++)
        {
            std::cout << "Gameplay " << it << std::endl;

            DE_DEBUG("(EAScript) Requesting game to start");
            m_GameRunner.Start();

            DE_DEBUG("(EAScript) LOCK 1");
            endSem.Wait();
            DE_DEBUG("(EAScript) LOCK 2");
            endSem.Wait();
            DE_DEBUG("(EAScript) UNLOCK 2");
            endSem.Post();
            it++;
        }

        return gameplaysResults;
    }

} // namespace EAAlgorithm

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