#pragma once

#include <string>

namespace Application
{
    enum class RobotFunction : short
    {
        PROTECTION = 0,
        HUNT,
        MEDICINE,
        CONSTRUCTION,
        RESOURCE_GATHERING
    };

    const std::string& getRobotFunctionString(RobotFunction function);
}