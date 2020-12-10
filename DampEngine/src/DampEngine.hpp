#pragma once

#include "DampEngine/Core/PlatformDetection.hpp"

#ifdef  DE_ENGINE_BUILD
    #error "Core modules shouldn't include the export header file"
#endif  //DE_ENGINE_BUILD

/************************************************************************
 **                                                                    **
 **  THIS IS THE MAIN HEADER TO INCLUDE (NOT USED BY ANY ENGINE FILE)  **
 **                                                                    **
 ************************************************************************/

//----------- INCLUDE MACROS OPTIONS -----------//
#ifdef  DE_MACROS_DISABLE_ALL
    #define DE_MACROS_LOG_DISABLE
    #define DE_MACROS_ASSERT_DISABLE
#endif  //DE_MACROS_DISABLE_ALL

//----------- INCLUDE NEEDED MODULES -----------//
#include "DampEngine/Core/Application.hpp"
#include "DampEngine/Core/Layer.hpp"
#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Core/Log.hpp"

#include "DampEngine/Events.hpp"

#include "DampEngine/Core/KeyCodes.hpp"
#include "DampEngine/Core/Input.hpp"

#include "DampEngine/ImGui/ImGuiLayer.hpp"

//----- ENTRY POINT -----//
#include "DampEngine/Core/EntryPoint.hpp"


//Include Linux Window if running system is linux
#ifdef DE_PLATFORM_LINUX
    #include "Platform/Linux/LinuxGLFWWindow.hpp"
#endif  //DE_SYSYEM_LINUX



//----------- INCLUDE MACROS ACCORDING TO OPTIONS -----------//
#ifndef DE_MACROS_LOG_DISABLE
    #include "DampEngine/Core/Macros/Log.hpp"
#endif  //DE_MACROS_LOG_DISABLE


#ifndef  DE_MACROS_ASSERT_DISABLE
    #include "DampEngine/Core/Macros/Assert.hpp"
#endif  //DE_MACROS_ASSERT_DISABLE