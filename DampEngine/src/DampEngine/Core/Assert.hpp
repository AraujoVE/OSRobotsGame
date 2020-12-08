#pragma once

#include <DampEngine/Core/Log.hpp>

#define DE_ASSERT(check, message) { if (!(check)) DE_ERROR("Assertion failed: '{0}'", #check); }
#define DE_ENGINE_ASSERT(check, message) { if (!(check)) DE_ENGINE_ERROR("Assertion failed: '{0}'", #check); }