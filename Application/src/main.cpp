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

#include <DampEngine.hpp>
#include <DampEngine/Core/EntryPoint.hpp>

#include <iostream>

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application("OSRobotsGame") {}

    ~MyApplication() override {}

    void Run() override
    {
        std::cout << "Run method Called" << std::endl;
        DampEngine::Logger::GetClientLogger()->info("Run method Called");
        DE_ASSERT(0 == 1, "oloko");
    }
};

extern DampEngine::Application *CreateApplication() {
    std::cout << "CreateApplication Called" << std::endl;
    DampEngine::Logger::GetClientLogger()->info("CreateApplication Called");
    return new MyApplication();
}

