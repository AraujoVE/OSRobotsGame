#include "header/SDL.hpp"

#include "header/VillageStats.hpp"
#include "header/RobotsManagement.hpp"
#include "header/Game.hpp"
#include "header/GameRenderer.hpp"

#include <iostream>
#include <exception>

SDL_Window *initSDLAndCreateMainWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error(std::string("error initializing SDL:") + SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("Teste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    if (window == nullptr) { throw std::runtime_error("Unable to create SDL Window"); }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) { throw std::runtime_error("Unable to create SDL Renderer"); }


    //Paint it all black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return window;
}

void gameLoop(Game* game) {
    
    while (true)
    {   
        game->getRenderer()->renderGame();
        SDL_Delay(500);
    }
}

void startGame() {
    SDL_Window *gameWindow = initSDLAndCreateMainWindow();
    SDL_Renderer *windowRenderer = SDL_GetRenderer(gameWindow);

    //TODO: remove debug
    SDL_Delay(1000);

    Game *game = new Game(windowRenderer);

    //TODO: load game

    gameLoop(game);

    //TODO: save

    delete game;
    SDL_Quit();

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