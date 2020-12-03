#pragma once

#include "SDL.hpp"

#include "Game.fwd.hpp"
#include "GameRenderer.fwd.hpp"

#include "VillageStats.hpp"
#include "RobotsManagement.hpp"

class Game {
    VillageStats *villageStats;
    RobotsManagement *robotMan;
    GameRenderer *gameRenderer;

public:
    Game(SDL_Renderer *renderer);
    ~Game();

    void loadGame();
    void saveGame();

    const GameRenderer* getRenderer();
};