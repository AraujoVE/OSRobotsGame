#pragma once

// #include "Application/Events/Events.fwd.hpp"
#include "Application/Game/GameSave.hpp"

namespace Application
{

    struct GameStatus
    {
        bool
            GameStarted = false,
            GamePaused = false,
            GameLost = false;

        std::string GameLostReason = "Unexpected State! this is an error!";
    };

    class EH_GameStarted;
    class EH_GameEnded;
    class EventListener;
    class GameRunner final
    {  
    public:
        GameRunner(GameConsts *gameConsts);
        GameRunner(const std::shared_ptr<GameSave> &gameSave);
        ~GameRunner();

        void SetOnGameStarted(EH_GameStarted *eventHandler);
        void SetOnGameEnded(EH_GameEnded *eventHandler);

        void Start();
        void Stop();
        void ResetSave();

        void Pause();
        void Unpause();

        void OnGameLost(const std::string &reason);

        inline bool IsGamePaused() const { return m_GameStatus.GamePaused && m_GameStatus.GameStarted; };

        inline bool IsGameLost() const { return m_GameStatus.GameLost; };
        inline const std::string &GetGameLostReason() const { return m_GameStatus.GameLostReason; };

        inline GameSave &GetSave() { return *m_GameSave.get(); }

        inline GameConsts &GetGameConsts() { return *m_GameConsts; }

    private:
        void SetupGameOverConditions();

    private:
        std::shared_ptr<GameSave> m_GameSave;
        GameConsts *m_GameConsts;
        EventListener *m_EventListener;

        GameStatus m_GameStatus;
    };
} // namespace Application
