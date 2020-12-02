#pragma once

#include "SDL.hpp"

#include "VillageStats.hpp"
#include "RobotsManagement.hpp"
#include "GameRenderer.hpp"

class GameRenderer;
class Game {
    VillageStats *villageStats;
    RobotsManagement *robotMan;
    GameRenderer *gameRenderer;

public:
    Game(SDL_Renderer *renderer) {
        this->villageStats = new VillageStats();
        this->robotMan = new RobotsManagement();
        // this->gameRenderer = new GameRenderer(*this, renderer);
    }   

    ~Game() {
        delete villageStats;
        delete robotMan;
    }

    void loadGame();
    void saveGame();

    const GameRenderer* getRenderer() { return gameRenderer; }
};