#pragma once

#include <memory>

namespace Application
{
    //Forward declaration to boost compilation time
    class VillageStats;
    class RobotsManagement;
    class GameSave final
    {
    private:
        std::unique_ptr<VillageStats> m_VillageStats;
        std::unique_ptr<RobotsManagement> m_RobotsManagement;

    public:
        GameSave();
        ~GameSave();

        void Load();
        void Save() const;
        void Reset();

        std::unique_ptr<VillageStats> &getVillageStats(); 
        std::unique_ptr<RobotsManagement> &getRobotsManagement();
    };
} // namespace Application
