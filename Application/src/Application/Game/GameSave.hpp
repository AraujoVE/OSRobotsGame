#pragma once

#include <memory>

namespace Application
{
    //Forward declaration to boost compilation time
    class VillageStats;
    class RobotsManagement;
    class GameConsts;
    class GameSave final
    {
    private:
        GameConsts *m_GameConsts;
        std::unique_ptr<VillageStats> m_VillageStats;
        std::unique_ptr<RobotsManagement> m_RobotsManagement;

        //Exclusive for Loading gamesaves
        GameSave();

    public:
        GameSave(GameConsts *gameConsts);
        ~GameSave();

        static void Load();
        static void Save();
        void Reset();

        std::unique_ptr<VillageStats> &GetVillageStats();
        std::unique_ptr<RobotsManagement> &GetRobotsManagement();
        inline GameConsts &GetGameConsts() { return *m_GameConsts; }
    };
} // namespace Application
