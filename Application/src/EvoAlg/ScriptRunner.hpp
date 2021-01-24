#pragma once

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"
#include "DampEngine/Core/Macros/Log.hpp"
#include "DampEngine/Core/Macros/Assert.hpp"

#include "EvoAlg/Script.hpp"
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
    struct ScriptFunctions {
        GameRunner &m_GameRunner;

        const static int HUMAN_OP_DELAY = 500000;
        const static int OPERATION_TYPE_COUNT = 7;

        void scriptFunct0(const std::vector<std::string> &params);
        void scriptFunct1(const std::vector<std::string> &params);
        void scriptFunct2(const std::vector<std::string> &params);
        void scriptFunct3(const std::vector<std::string> &params);
        void scriptFunct4(const std::vector<std::string> &params);
        void scriptFunct5(const std::vector<std::string> &params);
        void scriptFunct6(const std::vector<std::string> &params);

        void (ScriptFunctions::*scriptLoopFuncts[OPERATION_TYPE_COUNT])(const std::vector<std::string> &params) = {
            &ScriptFunctions::scriptFunct0,
            &ScriptFunctions::scriptFunct1,
            &ScriptFunctions::scriptFunct2,
            &ScriptFunctions::scriptFunct3,
            &ScriptFunctions::scriptFunct4,
            &ScriptFunctions::scriptFunct5,
            &ScriptFunctions::scriptFunct6};

        inline void Execute(const std::vector<std::string>& operation) {
            (this->*(scriptLoopFuncts[stoi(operation.at(0))]))(operation);
        }

        ScriptFunctions(GameRunner &gameRunner): m_GameRunner(gameRunner) {}
    };


    class ScriptRunner
    {
    private:
        Script &m_Script;
        GameRunner &m_GameRunner;
        const Individual& m_Individual;

        pthread_t scriptThread;
        std::vector<std::vector<std::vector<std::string>>> m_GameScript;

        const static int HUMAN_GP_TICK_DELAY = 500000;
        static constexpr double AU_PER_TICK = HUMAN_GP_TICK_DELAY/1e6;

        ScriptFunctions m_OperationFunctions;


    public:
        ScriptRunner(Script& script, GameRunner &gameRunner, const Individual& individual);

        ~ScriptRunner();

        void initScriptDirections();

        
        std::vector<TimeResult> *RunAllGameplays();
        TimeResult RunGameplay(uint64_t gameplayIndex);
       

    };
} // namespace EvoAlg