#pragma once

//Forward declaration to boost compilation time
class VillageStats;
class RobotsManagement;

#include <memory>

namespace Application
{
    class GameSave final
    {
    private:
        std::unique_ptr<VillageStats> villageStats;
        std::unique_ptr<RobotsManagement> robotsManagement;

    public:
        GameSave()
        {
            clear();
        }
        ~GameSave() {}

        void loadGame();
        void saveGame() const;
        void clear() { 
            villageStats.reset(new VillageStats());
            robotsManagement.reset(new RobotsManagement());
            robotsManagement->setVillageStats(villageStats.get());
        }

        inline std::unique_ptr<VillageStats> &getVillageStats() { return villageStats; }
        inline std::unique_ptr<RobotsManagement> &getRobotsManagement() { return robotsManagement; }
    };

} // namespace Application
