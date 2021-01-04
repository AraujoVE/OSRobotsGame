#ifndef EA_SCRIPT
#define EA_SCRIPT

#include <pthread.h>

namespace Application{
    class EAScript{
        private:
            //TODO: Make RobotCreationWindow accesible
            FunctionWindow *m_functionWindows[FUNCTION_QTY];
            RobotCreationWindow *m_robotCreationWindow;
            std::string srcFile;
            pthread_t scriptThread;
            std::vector<std::vector<std::vector<std::string>>> scriptDirections;
            const static int DIRECTIONS_SIZE = 7;
            const static int WAIT_UNIT = 500000;
        public:
            EAScript(FunctionWindow*[FUNCTION_QTY],RobotCreationWindow*,std::string);
            ~EAScript();
            void scriptLoop();
            
            void direction0(std::vector<std::string>);
            void direction1(std::vector<std::string>);
            void direction2(std::vector<std::string>);
            void direction3(std::vector<std::string>);
            void direction4(std::vector<std::string>);
            void direction5(std::vector<std::string>);
            void direction6(std::vector<std::string>);

            void (EAScript::*scriptLoopFuncts[DIRECTIONS_SIZE])(std::vector<std::string>) = {
                &EAScript::direction0,
                &EAScript::direction1,
                &EAScript::direction2,
                &EAScript::direction3,
                &EAScript::direction4,
                &EAScript::direction5,
                &EAScript::direction6
            };

            void initScriptDirections();




    };
    void *runScript(void*);
}



#endif