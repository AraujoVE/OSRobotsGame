#include "mypch.hpp"
namespace Application{
    EAScript::EAScript(FunctionWindow *functionWindows[FUNCTION_QTY],RobotCreationWindow *robotCreationWindow,std::string filePath): 
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
        int tokenPos,curOp = -1,curDirect;
        myFile.open(srcFile, std::ios::in);
        std::string line;
        while (std::getline(myFile, line))
        {
            if(line[0] == '#'){
                scriptDirections.push_back(std::vector<std::vector<std::string>>());
                curOp++;
                curDirect = -1;
            }
            else{
                scriptDirections.at(curOp).push_back(std::vector<std::string>());
                curDirect++;
                tokenPos = line.find(",");
                while(tokenPos != -1){
                    scriptDirections.at(curOp).at(curDirect).push_back(line.substr(0,tokenPos));
                    line = line.substr(tokenPos + 1,line.length() - 1);
                    tokenPos = line.find(",");
                }
                scriptDirections.at(curOp).at(curDirect).push_back(line);
            }
        }
    }


    void EAScript::direction0(std::vector<std::string> params){
        RobotFunction function = static_cast<RobotFunction>(stoi(params.at(1)));
        m_functionWindows[stoi(params.at(1))]->CreateTask();
    }

    void EAScript::direction1(std::vector<std::string> params){
        m_functionWindows[stoi(params.at(1))]->OnTaskEnded(stoi(params.at(2)));
    }

    void EAScript::direction2(std::vector<std::string> params){
        //TODO:Just posses taskWindow, must make moveRobots work on buttons
        m_functionWindows[stoi(params.at(1))]->getTaskWindow(stoi(params.at(2)));
    }

    void EAScript::direction3(std::vector<std::string> params){
        //TODO:Just posses taskWindow, must make moveRobots work on buttons
        m_functionWindows[stoi(params.at(1))]->getTaskWindow(stoi(params.at(2)));
    }

    void EAScript::direction4(std::vector<std::string> params){
        //TODO:Just posses robotCreationWindow, must make createRobot work on buttons
        m_robotCreationWindow;
    }

    void EAScript::direction5(std::vector<std::string> params){
        //TODO:Just posses robotCreationWindow, must make destroyRobot work on buttons
        m_robotCreationWindow;
    }

    void EAScript::direction6(std::vector<std::string> params){
        int waitTime = (std::stoi(params.at(0)) > 1 ? std::stoi(params.at(0)) - 1 : 0);

        usleep(waitTime*WAIT_UNIT);
    }

    void EAScript::scriptLoop(){
        int start = -1;
        time_t initTime;
        
        while(true){
            for(auto gameplay : scriptDirections){
                initTime = time(0);
                for(auto instruction : gameplay){
                    (this->*(scriptLoopFuncts[stoi(instruction.at(0))]))(instruction);
                    usleep(WAIT_UNIT);
                }
                //EAFunction(initTime - time(0));
            }
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