#include "Application/Game/Ingame/RobotFunctions.hpp"

#include "mypch.hpp"

namespace Application
{
    const std::string& getRobotFunctionString(RobotFunction function) {
        static std::string robotFunctionString[] = {
            "PROTECTION",
            "HUNT",
            "MEDICINE",
            "CONSTRUCTION",
            "RESOURCE_GATHERING"
        };

        return robotFunctionString[(int)function];
    }

    
} // namespace Application
