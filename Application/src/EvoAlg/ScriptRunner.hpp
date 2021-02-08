#pragma once

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"
#include "DampEngine/Core/Macros/Log.hpp"
#include "DampEngine/Core/Macros/Assert.hpp"

#include "EvoAlg/Script.hpp"
#include "EvoAlg/Types.hpp"

#include <vector>

namespace Application{
    class GameSave;
    class GameRunner;
}
using namespace Application;

namespace EvoAlg
{
    struct ScriptFunctions {
        const static int HUMAN_OP_DELAY = 500000;
        const static int OPERATION_TYPE_COUNT = 7;

        void scriptFunct0(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct1(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct2(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct3(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct4(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct5(GameRunner&, const std::vector<std::string> &params);
        void scriptFunct6(GameRunner&, const std::vector<std::string> &params);

        void (ScriptFunctions::*scriptLoopFuncts[OPERATION_TYPE_COUNT])(GameRunner&, const std::vector<std::string> &params) = {
            &ScriptFunctions::scriptFunct0,
            &ScriptFunctions::scriptFunct1,
            &ScriptFunctions::scriptFunct2,
            &ScriptFunctions::scriptFunct3,
            &ScriptFunctions::scriptFunct4,
            &ScriptFunctions::scriptFunct5,
            &ScriptFunctions::scriptFunct6};

        void Execute(GameRunner& gameRunner, const std::vector<std::string>& operation);
    };


    class ScriptRunner
    {
    private:
        Script &m_Script;

        pthread_t scriptThread;
        std::vector<std::vector<std::vector<std::string>>> m_GameScript;

        const static int HUMAN_GP_TICK_DELAY = 500000;
        static constexpr double AU_PER_TICK = HUMAN_GP_TICK_DELAY/1e6;

        ScriptFunctions m_OperationFunctions;


    public:
        ScriptRunner(Script& script);

        ~ScriptRunner();

        void initScriptDirections();

        
        std::vector<TimeResult> *RunAllGameplays(GameRunner&, Individual&);
        TimeResult RunGameplay(GameRunner&, Individual&, uint64_t gameplayIndex);
       

    };
} // namespace EvoAlg