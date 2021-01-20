#pragma once

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"
#include "DampEngine/Core/Macros/Log.hpp"
#include "DampEngine/Core/Macros/Assert.hpp"

#include "EvoAlg/Types.hpp"

#include <vector>
#include <pthread.h>

namespace Application{
    class GameSave;
    class GameRunner;
}
using namespace Application;

namespace EvoAlg
{
    class ScriptRunner
    {
    private:
        GameRunner &m_GameRunner;
        std::string srcFile, m_DebugName;
        pthread_t scriptThread;
        std::vector<std::vector<std::vector<std::string>>> gameScript;
        const static int DIRECTIONS_SIZE = 7;
        const static int HUMAN_OP_DELAY = 500000;
        const static int HUMAN_GP_TICK_DELAY = 500000;
        static constexpr double AU_PER_TICK = HUMAN_GP_TICK_DELAY/1e6;


    public:
        ScriptRunner(GameRunner &gameRunner, const std::string& filePath, const std::string& debugName);

        ~ScriptRunner();

        void startScript();

        void scriptFunct0(const std::vector<std::string> &params);
        void scriptFunct1(const std::vector<std::string> &params);
        void scriptFunct2(const std::vector<std::string> &params);
        void scriptFunct3(const std::vector<std::string> &params);
        void scriptFunct4(const std::vector<std::string> &params);
        void scriptFunct5(const std::vector<std::string> &params);
        void scriptFunct6(const std::vector<std::string> &params);
        std::vector<TimeResult> *scriptLoop();

        void (ScriptRunner::*scriptLoopFuncts[DIRECTIONS_SIZE])(const std::vector<std::string> &) = {
            &ScriptRunner::scriptFunct0,
            &ScriptRunner::scriptFunct1,
            &ScriptRunner::scriptFunct2,
            &ScriptRunner::scriptFunct3,
            &ScriptRunner::scriptFunct4,
            &ScriptRunner::scriptFunct5,
            &ScriptRunner::scriptFunct6};

        void initScriptDirections();

        std::vector<TimeResult> *joinScriptThread() 
        { 
            void *ret;
            pthread_join(scriptThread, &ret); 
            DE_ASSERT(ret != nullptr, "Wrong thread return (null)");
            return (std::vector<TimeResult>*) ret;
        }
    };
} // namespace EvoAlg