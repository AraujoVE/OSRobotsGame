#ifndef EA_SCRIPT
#define EA_SCRIPT

#include <pthread.h>

namespace Application{
    class RobotCreationWindow;
    class FunctionWindow;
    class GameSave;
    class EAScript{
        private:
            //TODO: Make RobotCreationWindow accesible
            GameSave &m_gameSave;
            FunctionWindow *m_functionWindows[FUNCTION_QTY];
            RobotCreationWindow *m_robotCreationWindow;
            std::string srcFile;
            pthread_t scriptThread;
            std::vector<std::vector<std::vector<std::string>>> gameScript;
            const static int DIRECTIONS_SIZE = 7;
            const static int WAIT_UNIT = 500000;
        public:
            EAScript(GameSave&,FunctionWindow*[],RobotCreationWindow*,std::string);
            ~EAScript();
            void scriptLoop();
            
            void scriptFunct0(std::vector<std::string>);
            void scriptFunct1(std::vector<std::string>);
            void scriptFunct2(std::vector<std::string>);
            void scriptFunct3(std::vector<std::string>);
            void scriptFunct4(std::vector<std::string>);
            void scriptFunct5(std::vector<std::string>);
            void scriptFunct6(std::vector<std::string>);

            void (EAScript::*scriptLoopFuncts[DIRECTIONS_SIZE])(std::vector<std::string>) = {
                &EAScript::scriptFunct0,
                &EAScript::scriptFunct1,
                &EAScript::scriptFunct2,
                &EAScript::scriptFunct3,
                &EAScript::scriptFunct4,
                &EAScript::scriptFunct5,
                &EAScript::scriptFunct6
            };

            void initScriptDirections();




    };
    void *runScript(void*);
}



#endif