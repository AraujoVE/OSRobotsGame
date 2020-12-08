#pragma once

#include "DampEngine/Core/Application.hpp"
#include "DampEngine/Core/Layer.hpp"
#include "DampEngine/Core/Log.hpp"
#include "DampEngine/Core/Assert.hpp"

#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/KeyEvent.hpp"
#include "DampEngine/Events/MouseButonEvent.hpp"
#include "DampEngine/Events/MouseMovedEvent.hpp"
#include "DampEngine/Events/MouseScrolledEvent.hpp"
#include "DampEngine/Events/WindowEvent.hpp"

// #ifdef  DE_SYSYEM_LINUX

#include "Platform/Linux/LinuxGLFWWindow.hpp"

// #endif  //DE_SYSYEM_LINUX