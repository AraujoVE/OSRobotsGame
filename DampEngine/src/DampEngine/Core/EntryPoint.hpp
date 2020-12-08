#pragma once

#include <DampEngine/Core/Base.hpp>
#include <DampEngine/Core/Application.hpp>

#include "depch.hpp"

//To be defined by client program
extern DampEngine::Application *CreateApplication();

int	main(int argc, char **argv)
{
    DampEngine::Logger::Init();

    DE_ENGINE_INFO("Staring DampEngine!");

    DE_ENGINE_DEBUG("Requesting extern function CreateApplication");
    auto app = CreateApplication();

    app->Run();

    DE_ENGINE_DEBUG("Deleting application");
    delete app;

    DE_ENGINE_INFO("DampEngine has been gracefully ended!");
}
