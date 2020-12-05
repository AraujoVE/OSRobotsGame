#include "header/SDL.hpp"

#include "header/VillageStats.hpp"
#include "header/RobotsManagement.hpp"
#include "header/Game.hpp"
#include "header/GameGraphics.hpp"

#include <stdexcept>

//TODO: move
bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
    if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
        return true;
    return false;
} 

int main(void){
    //Initialize all used SDL modules.    
    OSDL::initModules();

    Game *game = new Game();

    game->run();
    
    delete game;

    OSDL::stopModules();

    return 0;
}