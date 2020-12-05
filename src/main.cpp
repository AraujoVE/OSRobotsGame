#include "header/SDL.hpp"

#include "header/VillageStats.hpp"
#include "header/RobotsManagement.hpp"
#include "header/Game.hpp"
#include "header/GameGraphics.hpp"

#include <iostream>
#include <exception>

//TODO: move
SDL_Window *initSDLAndCreateMainWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error(std::string("error initializing SDL:") + SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        throw std::runtime_error(std::string("error initializing SDL_image's PNG module: ") + IMG_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Teste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    if (window == nullptr) { throw std::runtime_error("Unable to create SDL Window"); }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) { throw std::runtime_error("Unable to create SDL Renderer"); }


    return window;
}

//TODO: move
bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
    if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
        return true;
    return false;
} 

int main(void){
    SDL_Window *gameWindow = initSDLAndCreateMainWindow();
    Game *game = new Game(gameWindow);

    //TODO: load game
    game->start();
    //TODO: save game

    delete game;
    return 0;
}