#include "Application/Game/GameSave.hpp"

#include "Application/header/VillageStats.hpp"
#include "Application/header/RobotsManagement.hpp"
#include "Application/header/ConstsMap.hpp"

#include "mypch.hpp"

namespace Application
{

    GameSave::GameSave(): m_GameConsts(new GameConsts()) {}

    GameSave::GameSave(const std::string &gameConstsPath) : GameSave()
    {
        m_GameConsts->LoadValuesFromFile(gameConstsPath);
        Reset();
    }

    GameSave::~GameSave()
    {
        delete m_GameConsts;
    }

    void GameSave::Reset()
    {
        m_VillageStats.reset(new VillageStats(*m_GameConsts));
        m_RobotsManagement.reset(new RobotsManagement(*m_GameConsts));
        m_RobotsManagement->setVillageStats(m_VillageStats.get());
    }

    std::unique_ptr<VillageStats> &GameSave::GetVillageStats() { return m_VillageStats; }
    std::unique_ptr<RobotsManagement> &GameSave::GetRobotsManagement() { return m_RobotsManagement; }

    void GameSave::Load()
    {
    }

    void GameSave::Save()
    {
    }

} // namespace Application
