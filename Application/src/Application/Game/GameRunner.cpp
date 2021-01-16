#include "Application/Game/GameRunner.hpp"
#include "Application/header/RobotsManagement.hpp"
#include "Application/header/VillageStats.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Events/EventListener/EventListener.hpp"

#include "mypch.hpp"
namespace Application
{
    //PUBLIC:
    GameRunner::GameRunner() : GameRunner(std::make_shared<GameSave>())
    {
    }

    GameRunner::GameRunner(const std::shared_ptr<GameSave> &gameSave)
    {
        m_GameSave = gameSave;
        m_GameRunning = false;
        m_GameLost = false;
        m_EventListener = new EventListener();
    }

    GameRunner::~GameRunner()
    {
        delete m_EventListener;
    }

    void GameRunner::SetOnGameStarted(EH_GameStarted *eventHandler) { m_EventListener->Register(eventHandler); }
    void GameRunner::SetOnGameEnded(EH_GameEnded *eventHandler) { m_EventListener->Register(eventHandler); }

    void GameRunner::Start()
    {
        DE_ASSERT(!m_GameRunning, "Trying to start the game 2 times in the same runner");

        if (m_GameLost)
        {
            ResetSave();
            m_GameLost = false;
        }

        m_GameRunning = true;

        m_GameSave->GetVillageStats()->startStatsDecayment();

        SetupGameOverConditions();
        m_EventListener->On<EH_GameStarted>(*this);
    }

    void GameRunner::Stop()
    {
        DE_ASSERT(m_GameRunning, "Trying to stop a game that is not running");

        m_EventListener->On<EH_GameEnded>(*this);
        m_GameRunning = false;

        //TODO: stop village stats decayment
    }

    void GameRunner::Pause()
    {
        m_GameRunning = false;
        m_GameSave->GetVillageStats()->setStatsDecaymentPaused(true);
    }
    void GameRunner::Unpause()
    {
        m_GameRunning = true;
        m_GameSave->GetVillageStats()->setStatsDecaymentPaused(false);
    }

    void GameRunner::OnGameLost(const std::string &reason)
    {
        m_GameLost = true;
        m_GameLostReason = reason;
        Stop();
    }

    void GameRunner::ResetSave()
    {
        m_GameSave->Reset();
    }

    //PRIVATE:
    void GameRunner::SetupGameOverConditions()
    {
        auto *robotsManagement = m_GameSave->GetRobotsManagement().get();
        auto *villageStats = m_GameSave->GetVillageStats().get();

        robotsManagement->setOnRobotsDestroyed(new EH_RobotsDestroyed([=](int _) {
            if (robotsManagement->getTotRobots() <= 0 && villageStats->getResources() <= 0 && !IsGameLost())
            {
                this->OnGameLost("No more robots available!");
                return true;
            }

            return false;
        }));

        villageStats->setOnStatusDecayed(new EH_StatsDecayed([=]() {
            if (villageStats->getPopulation() <= 0 && !IsGameLost())
            {
                this->OnGameLost("Your population reached 0!");
                return true;
            }

            return false;
        }));
    }

} // namespace Application
