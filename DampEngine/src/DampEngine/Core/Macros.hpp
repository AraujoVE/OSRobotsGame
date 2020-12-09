#pragma once

#include "DampEngine/Core/Macros/Primitives.hpp"
#include "DampEngine/Core/Macros/Assert.hpp"
#include "DampEngine/Core/Macros/Events.hpp"
#include "DampEngine/Core/Macros/Log.hpp"

#define BIT(x) (1 << x)

#define DE_BIND_FN(x) std::bind(&x, this, std::placeholders::_1)
