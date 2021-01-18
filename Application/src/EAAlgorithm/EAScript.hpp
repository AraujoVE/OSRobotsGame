#pragma once

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/header/RobotFunctions.hpp"
#include "DampEngine/Core/Macros/Log.hpp"
#include "DampEngine/Core/Macros/Assert.hpp"
#include <vector>
#include <pthread.h>

namespace Application{
    class GameSave;
    class GameRunner;
}
using namespace Application;

namespace EAAlgorithm
{
    class EAScript
    {
    private:
        GameRunner &m_GameRunner;
        std::string srcFile;
        pthread_t scriptThread;
        std::vector<std::vector<std::vector<std::string>>> gameScript;
        const static int DIRECTIONS_SIZE = 7;
        const static int HUMAN_WAIT_UNIT = 500000;

    public:
        EAScript(GameRunner &, std::string);
        ~EAScript();

        void startScript();

        void scriptFunct0(const std::vector<std::string> &params);
        void scriptFunct1(const std::vector<std::string> &params);
        void scriptFunct2(const std::vector<std::string> &params);
        void scriptFunct3(const std::vector<std::string> &params);
        void scriptFunct4(const std::vector<std::string> &params);
        void scriptFunct5(const std::vector<std::string> &params);
        void scriptFunct6(const std::vector<std::string> &params);
        std::vector<std::pair<double, double>> *scriptLoop();

        void (EAScript::*scriptLoopFuncts[DIRECTIONS_SIZE])(const std::vector<std::string> &) = {
            &EAScript::scriptFunct0,
            &EAScript::scriptFunct1,
            &EAScript::scriptFunct2,
            &EAScript::scriptFunct3,
            &EAScript::scriptFunct4,
            &EAScript::scriptFunct5,
            &EAScript::scriptFunct6};

        void initScriptDirections();

        std::vector<std::pair<double,double>> *joinScriptThread() 
        { 
            void *ret;
            pthread_join(scriptThread, &ret); 
            DE_ASSERT(ret != nullptr, "Wrong thread return (null)");
            return (std::vector<std::pair<double,double>>*) ret;
        }
    };
} // namespace EAAlgorithm