#include "Game.hpp"
#include "GameGraphics.hpp"
#include "EventListener.hpp"


Game::Game() {
    //Game status related fields
    this->villageStats = new VillageStats();
    this->robotMan = new RobotsManagement();
    this->gameGraphics = new GameGraphics(*this);
}

Game::~Game() {
    delete villageStats;
    delete robotMan;
    delete gameGraphics;
}

GameGraphics* Game::getGraphics() const { 
    return gameGraphics; 
}

void Game::requestGameStop() {
    gameRunning = false;
}

void Game::run() {
    //Initialization
    gameRunning = true;
    gameGraphics->start();

    //TODO: load game

    //GameLoop
    while (gameRunning)
    {   
        this->getGraphics()->renderGame();
        SDL_Delay(1000/30); //1000ms / 30 fps
    }

    //TODO: save game

    //Cleanup/destruction of dependencies
    gameGraphics->stop();
}

VillageStats *Game::getVillageStatus() const { 
    return villageStats;
}

RobotsManagement *Game::getRobotManagement() const { 
    return robotMan;
}