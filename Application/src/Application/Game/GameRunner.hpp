#pragma once

// #include "Application/Events/Events.fwd.hpp"
#include "Application/Game/GameSave.hpp"

namespace Application
{
    class EventListener;
    class GameRunner final
    {

    public:
        GameRunner();
        GameRunner(GameRunner&&) = default;
        GameRunner(std::shared_ptr<GameSave> gameSave);
        ~GameRunner();

        void setOnGameStarted(void *eventHandler);
        void setOnGameEnded(void *eventHandler);

        void Start();
        void Stop();

        void OnGameLost(const std::string& reason);
        inline bool IsGameLost() const { return m_GameLost; };

        inline GameSave &GetSave() { return *m_GameSave.get(); } 

    private:
        void SetupGameOverConditions();
        void ResetSave();
        void GameThread();
        void Tick();
    private:
        std::shared_ptr<GameSave> m_GameSave;
        EventListener *m_EventListener;
        bool m_GameRunning;
        bool m_GameLost;
    };
} // namespace Application
