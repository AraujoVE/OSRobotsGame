#include "Game.hpp"
#include "GameGraphics.hpp"
#include "EventListener.hpp"


Game::Game(SDL_Window *window) {
    //Game status related fields
    this->villageStats = new VillageStats();
    this->robotMan = new RobotsManagement();
    
    this->gameGraphics = new GameGraphics(*this, window);
}

Game::~Game() {
    delete villageStats;
    delete robotMan;
    delete gameGraphics;
}

GameGraphics* Game::getGraphics() const { 
    return gameGraphics; 
}

void Game::quit() {
    //TODO: save game
    gameRunning = false;
    //TODO: gameGraphics.stop() or smth
    SDL_Quit();
}

void Game::start() {
    gameRunning = true;
    gameGraphics->init();
    gameLoop();
}

void Game::gameLoop() {
    while (gameRunning)
    {   
        this->getGraphics()->renderGame();
        SDL_Delay(1000/30); //1000ms / 30 fps
    }
}

VillageStats *Game::getVillageStatus() const { 
    return villageStats;
}

RobotsManagement *Game::getRobotManagement() const { 
    return robotMan;
}