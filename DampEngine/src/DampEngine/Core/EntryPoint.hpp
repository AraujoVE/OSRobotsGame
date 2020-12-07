#pragma once

#include "DampEngine/Core/Application.hpp"

//To be defined by client program
extern DampEngine::Application *CreateApplication();

int main(int argc, char const *argv[])
{
    DampEngine::Logger::Init();

    auto app = CreateApplication();

    app->Run();

    delete app;
}
