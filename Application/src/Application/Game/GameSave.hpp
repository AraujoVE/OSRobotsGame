#pragma once

namespace Application
{
    //Forward declaration to boost compilation time
    class VillageStats;
    class RobotsManagement;
    class GameConsts;
    class GameSave final
    {
    private:
        //Not this class' resposibility to free
        GameConsts *m_GameConsts;

        
        VillageStats* m_VillageStats;
        RobotsManagement* m_RobotsManagement;

        //Exclusive for Loading gamesaves
        GameSave();

    public:
        GameSave(GameConsts *gameConsts);
        ~GameSave();

        static void Load();
        static void Save();
        void Reset();

        inline VillageStats *GetVillageStats() { return m_VillageStats; }
        inline RobotsManagement *GetRobotsManagement() { return m_RobotsManagement; }
        inline GameConsts &GetGameConsts() { return *m_GameConsts; }
    };
} // namespace Application
