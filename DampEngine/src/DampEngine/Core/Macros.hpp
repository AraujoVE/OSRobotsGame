#pragma once

#include "DampEngine/Core/Log.h"

#define DE_ASSERT(check, message) { if (!(check)) DE_ERROR("Assertion failed: '{0}'", #check); }