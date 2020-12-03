#pragma once

#include "SDL.hpp"

#include "Game.fwd.hpp"
#include "GameGraphics.fwd.hpp"

#include "VillageStats.hpp"
#include "RobotsManagement.hpp"


class Game {
private:
    VillageStats *villageStats;
    RobotsManagement *robotMan;

    GameGraphics *gameGraphics;

    bool gameEnded;

public:
    Game(SDL_Window *window);
    ~Game();

    void quit();

    void loadGame();
    void saveGame() const;

    void gameLoop();

    GameGraphics *getGraphics() const;
};