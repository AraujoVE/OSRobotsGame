#pragma once

#include <DampEngine/Core/Log.hpp>

#define DE_ASSERT(check, message, param, ...) { if (!(check)) DE_ERROR("Assertion failed: '{0}'", __VA_ARGS__); }
#define DE_ASSERT(check, message) { if (!(check)) DE_ERROR(message); }
#define DE_ASSERT(check) DE_ASSERT(check, "Assertion failed: '{0}'", #check)

#define DE_ENGINE_ASSERT(check, message, param, ...) { if (!(check)) DE_ENGINE_ERROR("Assertion failed: '{0}'", __VA_ARGS__); }
#define DE_ENGINE_ASSERT(check, message) { if (!(check)) DE_ENGINE_ERROR(message); }
#define DE_ENGINE_ASSERT(check) DE_ENGINE_ASSERT(check, "Assertion failed: '{0}'", #check)
