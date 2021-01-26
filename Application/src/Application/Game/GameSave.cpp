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
        m_VillageStats = nullptr;
        m_RobotsManagement = nullptr;
        Reset();
    }

    GameSave::~GameSave()
    {
        delete m_VillageStats;
        delete m_RobotsManagement;
        m_VillageStats = nullptr;
        m_RobotsManagement = nullptr;
    }

    void GameSave::Reset()
    {
        DE_TRACE("(GameSave) Starting new GameSave");
        delete m_VillageStats;
        delete m_RobotsManagement;

        m_VillageStats = new VillageStats(*m_GameConsts);
        m_RobotsManagement = new RobotsManagement(*m_GameConsts);
        m_RobotsManagement->setVillageStats(m_VillageStats);



    }

  

    void GameSave::Load()
    {
    }

    void GameSave::Save()
    {
    }

} // namespace Application
