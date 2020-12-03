#include "Game.hpp"
#include "GameGraphics.hpp"
#include "EventListener.hpp"

#include <iostream>

Game::Game(SDL_Window *window) {
    //Game status related fields
    this->villageStats = new VillageStats();
    this->robotMan = new RobotsManagement();
    
    this->gameGraphics = new GameGraphics(*this, window);
    this->gameEnded = false;
}

Game::~Game() {
    delete villageStats;
    delete robotMan;
}

GameGraphics* Game::getGraphics() const { 
    return gameGraphics; 
}

void Game::quit() {
    //TODO: save game
    gameEnded = true;
    delete gameGraphics;
    SDL_Quit();
}

void Game::gameLoop() {
    while (!gameEnded)
    {   
        this->getGraphics()->renderGame();
        SDL_Delay(500);
    }
}
