#pragma once

#include "VillageStats.hpp"
#include "RobotsManagement.hpp"
// bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
//     if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
//         return true;
//     return false;
// }

#include <memory>

namespace Application
{
    class GameSave final
    {
    private:
        std::unique_ptr<VillageStats> villageStats;
        std::unique_ptr<RobotsManagement> robotManagement;

    public:
        GameSave(){}
        ~GameSave(){}

        void loadGame();
        void saveGame() const;

        inline VillageStats &getVillageStats() const  { return *villageStats; }
        inline RobotsManagement &getRobotManagement() const  { return *robotManagement; }
    };

} // namespace Application
