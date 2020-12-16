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

    const std::string RobotFunctionString[] = {
        "PROTECTION",
        "HUNT",
        "MEDICINE",
        "CONSTRUCTION",
        "RESOURCE_GATHERIN"
    };      
}