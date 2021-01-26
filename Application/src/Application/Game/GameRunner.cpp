#include "Application/Game/GameRunner.hpp"
#include "Application/Game/Ingame/RobotsManagement.hpp"
#include "Application/Game/Ingame/VillageStats.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Game/GameConsts.hpp"

#include "Application/Util/path.hpp"

#include "mypch.hpp"
namespace Application
{
    //PUBLIC:
    //TODO: fix ml on new GameConsts()
    GameRunner::GameRunner(GameConsts *gameConsts) : m_GameSave(new GameSave(gameConsts)), m_GameConsts(gameConsts), m_EventListener(new EventListener())
    {
    }

    GameRunner::~GameRunner()
    {
        delete m_EventListener;
        delete m_GameSave;
    }

    //TODO: use promises to call EH_GameStarted to assure game is totally started 
    void GameRunner::Start()
    {
        DE_ASSERT(!m_GameStatus.GameStarted, "Trying to start the game 2 times in the same runner");

        DE_ASSERT(m_GameSave->GetVillageStats()->GetElapsedTimeTicks() == 0, "VillageStats is in an invalid statem, more than 0 ticks have been done");
        DE_ASSERT(m_GameSave->GetVillageStats()->GetElapsedTimeTicks() == 0, "VillageStats is already decaying before game start");
        // DE_ASSERT() ROBOTMAN

        //TODO: if someday the game needs to be saved, this will need to change
        if (m_GameStatus.GameLost || true)
        {
            ResetSave();
            m_GameStatus.GameLost = false;
        }

        m_GameStatus.GameStarted = true;
        m_GameStatus.GamePaused = false;
        
        
        m_EventListener->OnAsync<EH_GameStarted>(*this);
        
        SetupGameOverConditions();
        //TODO: register villagestats::onGameStarted on GameRunner::m_EventListener ?
        m_GameSave->GetVillageStats()->onGameStarted();
    }

    void GameRunner::Stop()
    {
        DE_ASSERT(m_GameStatus.GameStarted, "Trying to stop a game that is not running");

        m_GameStatus.GameStarted = false;


        m_GameSave->GetVillageStats()->onGameEnded();

        m_EventListener->OnAsync<EH_GameEnded>({*this, m_GameSave->GetVillageStats()->GetElapsedTimeTicks()});
        //TODO: promise to join TASK + VS ended callbacks
    }

    void GameRunner::Pause()
    {
        m_GameStatus.GamePaused = true;
        m_GameSave->GetVillageStats()->setStatsDecaymentPaused(true);
    }
    void GameRunner::Unpause()
    {
        m_GameStatus.GamePaused = false;
        m_GameSave->GetVillageStats()->setStatsDecaymentPaused(false);
    }

    void GameRunner::OnGameLost(const std::string &reason)
    {
        m_GameStatus.GameLost = true;
        m_GameStatus.GameLostReason = reason;
        Stop();
    }

    void GameRunner::ResetSave()
    {
        m_GameSave->Reset();
    }

    //PRIVATE:
    void GameRunner::SetupGameOverConditions()
    {
        auto *robotsManagement = m_GameSave->GetRobotsManagement();
        auto *villageStats = m_GameSave->GetVillageStats();


        const static std::string noRobotsReason = "No more robots available!";
        const static std::string popuDeadReason = "Your population reached 0";

        robotsManagement->setOnRobotsDestroyed(new EH_RobotsDestroyed([=](int _) {
            if (robotsManagement->getTotRobots() <= 0 && villageStats->getResources() <= 0 && !IsGameLost())
            {
                this->OnGameLost(noRobotsReason);
            }

            return false;
        }));

        villageStats->RegisterOnPopReachZero(new EH_DecaymentStopped([=]() {
            DE_TRACE("(GameRunner) Received EH_DecaymentStopped");
            if (villageStats->getPopulation() <= 0 && !IsGameLost())
            {
                this->OnGameLost(popuDeadReason);
            } else {
                DE_TRACE("(GameRunner) Ignoring EH_DecaymentStopped, POP={0}, GameLost={1}", villageStats->getPopulation(), IsGameLost());
            }

            return false;
        }));
    }

} // namespace Application
