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
        DE_DEBUG("Deleting m_EventListener @GameRunner::~GameRunner()");
        delete m_EventListener;
        DE_DEBUG("Deleting m_GameSave @GameRunner::~GameRunner()");
        delete m_GameSave;
    }

    //TODO: use promises to call EH_GameStarted to assure game is totally started
    void GameRunner::Start()
    {

        m_GSMutex.lock();
        // DE_ASSERT(!m_GameStatus.GameStarted, "Trying to start the game 2 times in the same runner");

        //TODO: if someday the game needs to be saved, this will need to change
        // if (m_GameStatus.GameLost)
        // {
            ResetSave();
            m_GameStatus.GameLost = false;
        // }

        VillageStats &villageStats = *m_GameSave->GetVillageStats();
        // SetupGameOverConditions();

        std::promise<void> gameStartedPr;
        std::future<void> gameStartedFut = gameStartedPr.get_future();
        EH_DecaymentStarted *eventHandler = new EH_DecaymentStarted([&gameStartedPr] {
            gameStartedPr.set_value();
            return false;
        });

        villageStats.RegisterOnStatsDecaymentStarted(eventHandler);
        villageStats.onGameStarted();
        
        DE_DEBUG("[GameRunner] Waiting for VillageStats to start decaymen...");
        gameStartedFut.get();
        DE_DEBUG("[GameRunner] VillageStats decayment started!");
        

        m_GameStatus.GameStarted = true;
        m_GameStatus.GamePaused = false;

        // DE_TRACE("Gamerunner finished starting");
        m_GSMutex.unlock();

        // m_EventListener->OnAsync<EH_GameStarted>(*this);
    }

    void GameRunner::Stop()
    {
        DE_TRACE("GameRunner::Stop()");
        if (!m_GameStatus.GameStarted)
            return;

        {
            std::lock_guard<std::mutex> gsGuard(m_GSMutex);
            auto elapsedTicks = m_GameSave->GetVillageStats()->GetElapsedTimeTicks();

            // DE_ASSERT(m_GameStatus.GameStarted, "Trying to stop a game that is not running (somehow)");

            m_GameStatus.GameStarted = false;

            DE_TRACE("Stopping game: 0/3 @GameRunner::Stop()");
            m_GameSave->GetRobotsManagement()->clearEvents();
            DE_TRACE("Stopping game: 1/3 @GameRunner::Stop()");
            m_GameSave->GetVillageStats()->ClearEvents();
            DE_TRACE("Stopping game: 2/3 @GameRunner::Stop()");
            m_GameSave->GetVillageStats()->onGameEnded();
            DE_TRACE("Stopping game: 3/3 @GameRunner::Stop()");
        }

        DE_TRACE("Emmiting EH_GameEnded @GameRunner::Stop()");
        // m_EventListener->OnAsync<EH_GameEnded>({*this, elapsedTicks});
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
        DE_INFO("(GameRunner::OnGameLost) Game Over! ['{0}']", reason);
        m_GSMutex.lock();
        m_GameStatus.GameLost = true;
        m_GameStatus.GameLostReason = reason;
        m_GSMutex.unlock();

        Action<> stopAction([=] { Stop(); });
        stopAction.Invoke();
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
            // m_GSMutex.lock();
            // DE_ASSERT(m_GameStatus.GameStarted, "(GameRunner) EH_RobotsDestroyed before game started?????");
            // m_GSMutex.unlock();
            if (robotsManagement->getTotRobots() <= 0 && villageStats->getResources() <= 0 && !IsGameLost())
            {
                this->OnGameLost(noRobotsReason);
            }

            return false;
        }));

        //TODO?: event passes a reason (why it has stopped)
        villageStats->RegisterOnStatsDecaymentStopped(new EH_DecaymentStopped([=]() {
            // m_GSMutex.lock();
            // DE_ASSERT(m_GameStatus.GameStarted, "(GameRunner) EH_DecaymentStopped before game started?????");
            // m_GSMutex.unlock();
            if (villageStats->getPopulation() <= 0 && !IsGameLost())
            {
                this->OnGameLost(popuDeadReason);
            }
            else
            {
                DE_TRACE("(GameRunner) Ignoring EH_DecaymentStopped, POP={0}, GameLost={1}", villageStats->getPopulation(), IsGameLost());
            }

            return false;
        }));
    }

} // namespace Application
