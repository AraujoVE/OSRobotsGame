#pragma once

#include <string>

#define RBF_STR(x) #x

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
        RBF_STR(HUNT),
        RBF_STR(MEDICINE),
        RBF_STR(CONSTRUCTION),
        RBF_STR(PROTECTION),
        RBF_STR(RESOURCE_GATHERING)
    };      
}

#undef RBF_STR