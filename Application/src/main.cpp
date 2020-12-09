// #include "SDL.hpp"

// #include "VillageStats.hpp"
// #include "RobotsManagement.hpp"
// #include "Game.hpp"
// #include "GameGraphics.hpp"

// #include <stdexcept>

// #include <iostream>

// //TODO: move
// bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
//     if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
//         return true;
//     return false;
// }

// int _main(void){
//     //Initialize all used SDL modules.
//     OSDL::initModules();

//     Game *game = new Game();

//     // game->run();

//     delete game;

//     OSDL::stopModules();

//     //TODO: use override in classes and more virtual destructors

//     return 0;
// }

// #define __DE_NO_MACROS__
#include <DampEngine.hpp>
#include <DampEngine/Core/EntryPoint.hpp>


// class MyApplication : public DampEngine::Application
// {
// public:
//     MyApplication(): DampEngine::Application("OSRobotsGame") {}

//     bool OnWindowClosed(DampEngine::WindowClosedEvent &event) override { return true; }
//     bool OnWindowResized(DampEngine::WindowResizedEvent &event) override { return true; }

//     ~MyApplication() override {}
// };

DampEngine::Application *CreateApplication() {
    DE_DEBUG("Creating application");
    return new DampEngine::Application();
}
