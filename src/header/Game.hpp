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

    bool gameRunning;

public:
    Game();
    ~Game();

    void run();
    void requestGameStop();

    void loadGame();
    void saveGame() const;

    GameGraphics *getGraphics() const;
    VillageStats *getVillageStatus() const;
    RobotsManagement *getRobotManagement() const;

};