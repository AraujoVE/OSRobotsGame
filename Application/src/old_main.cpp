// #include "header/SDL.hpp"

#ifdef  UNSET_PLEASE_HELP

#include "header/VillageStats.hpp"
#include "header/RobotsManagement.hpp"
#include "header/Game.hpp"

#include <iostream>
#include <exception>

// SDL_Window *initSDLAndCreateMainWindow() {
//     if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//         throw std::runtime_error(std::string("error initializing SDL:") + SDL_GetError());
//     }
//     SDL_Window* window = SDL_CreateWindow("Teste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
//     if (window == nullptr) { throw std::runtime_error("Unable to create SDL Window"); }

//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
//     if (renderer == nullptr) { throw std::runtime_error("Unable to create SDL Renderer"); }


//     //Paint it all black
//     SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
//     SDL_RenderClear(renderer);
//     SDL_RenderPresent(renderer);
//     SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

//     return window;
// }

void gameLoop(Game* game, VillageStats * village, RobotsManagement * robotsMng) {
    
    while (true)
    {   
        // Check if any task is completed (don't now if this should be in main)
        // Maybe we could use something that orders the tasks...
        for (std::map<int, Task> tasksMap : robotsMng->getTasks()) {
            for (auto const& taskInfo : tasksMap) {
                if (taskInfo.second.getPredictedTime() <= time(0)) { // check if task has already finished
                    printf("[*] Task with ID %d (category: %d) has finished!\n", taskInfo.first, taskInfo.second.getType());
                        // Add resources to the village stats
                        village->addTaskResources(taskInfo.second.getType(), taskInfo.second.getRobotsNo(), taskInfo.second.getInitTime());
                        // TODO: continue this part!
                }
            }
        }


        // Calculate new population
        village->calcNewPop();

        // Check if game is over
        // if(isGameOver(village, robotsMng))
        //     return; // game is over (in the future display a "Game Over message" here)

        // game->getRenderer()->renderGame();

        // SDL_Delay(500);
    }
}

void startGame() {
    // SDL_Window *gameWindow = initSDLAndCreateMainWindow();
    // SDL_Renderer *windowRenderer = SDL_GetRenderer(gameWindow);

    VillageStats village = VillageStats(); // create object and call constructor -> one village only!
    RobotsManagement robotsMng = RobotsManagement(); // same as here ^
    
    //TODO: remove debug
    // SDL_Delay(1000);

    // Game *game = new Game(windowRenderer);

    //TODO: load game

    // gameLoop(game, &village, &robotsMng);

    //TODO: save

    // delete game;
    // SDL_Quit();

    return;
}

bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
    if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
        return true;
    return false;
} 


int main(void){
    startGame();
    return 0;
}

#endif  //UNSET_PLEASE_HELP