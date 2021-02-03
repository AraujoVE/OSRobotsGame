#pragma once

#include "Application/Game/GameSave.hpp"
#include "Application/Events/EventListener/EventListener.template.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "DampEngine/Threads/Mutex.hpp"

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
        ~GameRunner();

        inline void RegisterOnGameStarted(EH_GameStarted *eventHandler) { m_EventListener->Register(eventHandler); }
        inline void RegisterOnGameEnded(EH_GameEnded *eventHandler) { m_EventListener->Register(eventHandler); }
        inline void UnregisterOnGameStarted(EH_GameStarted *eventHandler) { m_EventListener->Unregister(eventHandler); }
        inline void UnregisterOnGameEnded(EH_GameEnded *eventHandler) { m_EventListener->Unregister(eventHandler); }

        void Start();
        void Stop();
        void ResetSave();

        void Pause();
        void Unpause();

        void OnGameLost(const std::string &reason);

        inline bool HasStarted() const { return m_GameStatus.GameStarted; }
        inline bool IsGamePaused() const { return m_GameStatus.GamePaused && m_GameStatus.GameStarted; };
        inline bool IsGameLost() const { return m_GameStatus.GameLost; };
        inline const std::string &GetGameLostReason() const { return m_GameStatus.GameLostReason; };

        inline GameSave &GetSave() { return *m_GameSave; }

        inline GameConsts &GetGameConsts() { return *m_GameConsts; }

    private:
        void SetupGameOverConditions();

    private:
        GameSave *m_GameSave;
        GameConsts *m_GameConsts;
        EventListener *m_EventListener;

        DampEngine::Mutex m_GSMutex;
        GameStatus m_GameStatus;
    };
} // namespace Application
