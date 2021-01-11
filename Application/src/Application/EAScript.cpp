#include "mypch.hpp"
namespace Application{
    EAScript::EAScript(GameSave& gameSave,FunctionWindow *functionWindows[FUNCTION_QTY],RobotCreationWindow *robotCreationWindow,std::string filePath): 
        m_gameSave(gameSave),
        m_functionWindows{
            functionWindows[0],
            functionWindows[1],
            functionWindows[2],
            functionWindows[3],
            functionWindows[4]
        },
        m_robotCreationWindow(robotCreationWindow),
        srcFile(filePath)
    {
        initScriptDirections();
        pthread_create(&scriptThread, NULL, runScript, this);
    }

    EAScript::~EAScript(){
        pthread_join(scriptThread, NULL);
    }

    void EAScript::initScriptDirections(){
        std::fstream myFile;
        int tokenPos,curGameplay = -1,curOp;
        myFile.open(srcFile, std::ios::in);
        std::string line;
        while (std::getline(myFile, line))
        {
            if(line[0] == '#'){
                gameScript.push_back(std::vector<std::vector<std::string>>());
                curGameplay++;
                curOp = 0;
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                gameScript.at(curGameplay).at(curOp).push_back(line.substr(1,line.length() - 1));
            }
            else{
                gameScript.at(curGameplay).push_back(std::vector<std::string>());
                curOp++;
                tokenPos = line.find(",");
                while(tokenPos != -1){
                    gameScript.at(curGameplay).at(curOp).push_back(line.substr(0,tokenPos));
                    line = line.substr(tokenPos + 1,line.length() - (1 + tokenPos));
                    tokenPos = line.find(",");
                }
                gameScript.at(curGameplay).at(curOp).push_back(line);
            }
        }
    }


    void EAScript::scriptFunct0(std::vector<std::string> params){
        m_gameSave.getRobotsManagement()->createTask(static_cast<RobotFunction>(stoi(params.at(1))));
    }

    void EAScript::scriptFunct1(std::vector<std::string> params){
        // m_gameSave.getRobotsManagement()->
            // getTasks(static_cast<RobotFunction>(stoi(params.at(1)))).
            // find(stoi(params.at(2)))->second->stop();
    }

    void EAScript::scriptFunct2(std::vector<std::string> params){
        Task& curTask = m_functionWindows[stoi(params.at(1))]->getTaskWindow(stoi(params.at(2)))->getTask();
        m_gameSave.getRobotsManagement()->moveRobot(curTask,1);
    }

    void EAScript::scriptFunct3(std::vector<std::string> params){
        Task& curTask = m_functionWindows[stoi(params.at(1))]->getTaskWindow(stoi(params.at(2)))->getTask();
        m_gameSave.getRobotsManagement()->moveRobot(curTask,-1);
    }

    void EAScript::scriptFunct4(std::vector<std::string> params){
        m_gameSave.getRobotsManagement()->createRobots(1);
    }

    void EAScript::scriptFunct5(std::vector<std::string> params){
        m_gameSave.getRobotsManagement()->destroyRobots(1);        
    }

    void EAScript::scriptFunct6(std::vector<std::string> params){
        int waitTime = (std::stoi(params.at(1)) > 1 ? std::stoi(params.at(1)) - 1 : 0);
        usleep(waitTime*WAIT_UNIT);
    }
    //TODO: Definir callback de jogo acabou
    //TODO: Callbacks da UI para atualizar a parte gráfica
    //TODO: Jogo versão normal ou calibração
    //TODO: Velocidade dos segundos do jogo -> slider 
    //TODO: Limitar Tasks

    void EAScript::scriptLoop(){
        time_t initTime,endTime;
        int fitness,it = 0;
        while(true){
            fitness = 0;
            for(auto gameplay : gameScript){
                initTime = time(0);
                std::cout << "Gameplay " << ++it << std::endl;
                for(int i=1;i < (int)gameplay.size();i++){
                    std::cout << "\tAction " << i << std::endl;
                    (this->*(scriptLoopFuncts[stoi(gameplay.at(i).at(0))]))(gameplay.at(i));
                    usleep(WAIT_UNIT);
                }
                std::cout << "Acabou a primeira Gameplay";
                //Callback para saber quando acaba o jogo
                //quando isso acontece : endTime = time(0)
                endTime = time(0);
                fitness += std::abs((int)(initTime - endTime));
            }
            //EAFunction(fitness);
        }
    }

    void *runScript(void *scriptFuncObject) {
        EAScript *script = (EAScript*) scriptFuncObject;
        script->scriptLoop();
        return NULL;
    }
}

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