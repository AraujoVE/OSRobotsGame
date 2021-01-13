#include "Application/Game/GameRunner.hpp"
#include "Application/header/RobotsManagement.hpp"
#include "Application/header/VillageStats.hpp"
#include "Application/Events/Events.hpp"
#include "Application/Events/EventListener.hpp"

#include "mypch.hpp"
namespace Application
{
    //PUBLIC:
    GameRunner::GameRunner() : GameRunner(std::make_shared<GameSave>())
    {
    }

    GameRunner::GameRunner(std::shared_ptr<GameSave> gameSave)
    {
        m_GameSave = gameSave;
        m_GameRunning = false;
        m_EventListener = new EventListener();
    }

    GameRunner::~GameRunner()
    {
        delete m_EventListener;
    }

    void GameRunner::setOnGameStarted(void *eventHandler) {} //m_EventListener.Register(eventHandler); }
    void GameRunner::setOnGameEnded(void *eventHandler) {}   //m_EventListener.Register(eventHandler); }

    void GameRunner::Start()
    {
        if (m_GameRunning)
        {
            DE_CRITICAL("Trying to start the game 2 times in the same runner");
            std::raise(SIGINT);
            return;
        }
        m_GameRunning = true;
        m_GameLost = false;

        SetupGameOverConditions();
    }

    void GameRunner::Stop()
    {
        m_GameRunning = false;
    }

    void GameRunner::OnGameLost(const std::string &reason)
    {
        m_GameLost = true;
        ResetSave();
        Stop();
    }

    //PRIVATE:
    void GameRunner::SetupGameOverConditions()
    {
        auto &robotsManagement = *m_GameSave->GetRobotsManagement().get();
        auto &villageStats = *m_GameSave->GetVillageStats().get();

        EH_RobotsDestroyed *e = new EH_RobotsDestroyed([](int a){
            DE_DEBUG("Callback working!!");
            return true;
        });

        robotsManagement.setOnRobotsDestroyed(e);

        // robotsManagement.setOnRobotsDestroyed(new EH_RobotsDestroyed([=](int _) {
        //     if (robotsManagement.getTotRobots() <= 0)
        //     {
        //         this->OnGameLost("No more robots available!");
        //         return true;
        //     }

        //     return false;
        // }));

        // villageStats.setOnStatusDecayed(new EH_StatsDecayed([=]() {
        //     if (villageStats.getPopulation() <= 0)
        //     {
        //         this->OnGameLost("Your population reached 0!");
        //         return true;
        //     }

        //     return false;
        // }));
    }

    void GameRunner::ResetSave()
    {
        m_GameSave->Reset();
    }

    void GameRunner::Tick()
    {
    }

} // namespace Application
