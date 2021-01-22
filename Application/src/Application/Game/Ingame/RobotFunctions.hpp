#pragma once

#include <string>

namespace Application
{
    const int FUNCTION_QTY = 5;

    enum class RobotFunction : uint8_t
    {
        PROTECTION = 0,
        HUNT,
        MEDICINE,
        CONSTRUCTION,
        RESOURCE_GATHERING
    };

    const std::string& getRobotFunctionString(RobotFunction function);
}