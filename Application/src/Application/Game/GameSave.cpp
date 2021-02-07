#include "Application/Game/GameSave.hpp"

#include "Application/Game/Ingame/VillageStats.hpp"
#include "Application/Game/Ingame/RobotsManagement.hpp"
#include "Application/Game/GameConsts.hpp"

#include "mypch.hpp"

namespace Application
{

    //TODO: fix ml
    //PRIVATE:
    GameSave::GameSave(): m_GameConsts(new GameConsts()) {}

    //PUBLIC:
    GameSave::GameSave(GameConsts *gameConsts) : m_GameConsts(gameConsts)
    {
        Reset();
    }

    GameSave::~GameSave()
    {
    }

    void GameSave::Reset()
    {
        DE_TRACE("(GameSave) Starting new GameSave");
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
