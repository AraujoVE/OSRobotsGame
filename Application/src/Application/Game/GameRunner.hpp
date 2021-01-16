#pragma once

// #include "Application/Events/Events.fwd.hpp"
#include "Application/Game/GameSave.hpp"

namespace Application
{
    class EH_GameStarted;
    class EH_GameEnded;
    class EventListener;
    class GameRunner final
    {

    public:
        GameRunner();
        GameRunner(const std::shared_ptr<GameSave>& gameSave);
        ~GameRunner();

        void SetOnGameStarted(EH_GameStarted *eventHandler);
        void SetOnGameEnded(EH_GameEnded *eventHandler);

        void Start();
        void Stop();
        void ResetSave();

        void Pause();
        void Unpause();

        void OnGameLost(const std::string& reason);
        inline bool IsGameLost() const { return m_GameLost; };
        inline bool IsGamePaused() const { return !m_GameRunning && !m_GameLost; };
        inline const std::string& GetGameLostReason() const { return m_GameLostReason; };

        inline GameSave &GetSave() { return *m_GameSave.get(); } 

    private:
        void SetupGameOverConditions();
    private:
        std::shared_ptr<GameSave> m_GameSave;
        EventListener *m_EventListener;
        bool m_GameRunning;
        bool m_GameLost;
        std::string m_GameLostReason;
    };
} // namespace Application
