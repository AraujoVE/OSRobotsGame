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
            villageStats.reset(new VillageStats());
            robotsManagement.reset(new RobotsManagement());
            robotsManagement->setVillageStats(villageStats.get());
        }
        ~GameSave() {}

        void loadGame();
        void saveGame() const;

        inline VillageStats &getVillageStats() const { return *villageStats; }
        inline RobotsManagement &getRobotsManagement() const { return *robotsManagement; }
    };

} // namespace Application
