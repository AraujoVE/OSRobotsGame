#pragma once

#include "SDL.hpp"

#include "Game.fwd.hpp"
#include "GameGraphics.fwd.hpp"

#include "VillageStats.hpp"
#include "RobotsManagement.hpp"
// bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
//     if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
//         return true;
//     return false;
// }

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