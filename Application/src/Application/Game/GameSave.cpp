#include "Application/Game/GameSave.hpp"

#include "Application/header/VillageStats.hpp"
#include "Application/header/RobotsManagement.hpp"

#include "mypch.hpp"

namespace Application
{
    GameSave::GameSave()
    {
        Reset();
    }

    GameSave::~GameSave() {}

    void GameSave::Load()
    {
    }

    void GameSave::Save() const
    {
    }

    void GameSave::Reset()
    {
        m_VillageStats.reset(new VillageStats());
        m_RobotsManagement.reset(new RobotsManagement());
        m_RobotsManagement->setVillageStats(m_VillageStats.get());
    }

    inline std::unique_ptr<VillageStats> &GameSave::getVillageStats() { return m_VillageStats; }
    inline std::unique_ptr<RobotsManagement> &GameSave::getRobotsManagement() { return m_RobotsManagement; }

} // namespace Application
