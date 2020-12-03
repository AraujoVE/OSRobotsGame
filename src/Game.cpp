#include "header/Game.hpp"
#include "header/GameRenderer.hpp"

Game::Game(SDL_Renderer *renderer) {
        this->villageStats = new VillageStats();
        this->robotMan = new RobotsManagement();
        this->gameRenderer = new GameRenderer(*this, renderer);
}   

Game::~Game() {
        delete villageStats;
        delete robotMan;
}

const GameRenderer* Game::getRenderer() { 
    return gameRenderer; 
}
