#pragma once

#include "EvoAlg/Types.hpp"
#include "Application/Game/Types.fwd.hpp"
#include "Application/ImGui/Types.fwd.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include <unordered_map>
#include <optional>

namespace EvoAlg
{
    class EAGuiProps
    {

        //TODO: make multi-game (multi-thread) support
        struct EAGuiSettings {
            bool Paused = false;

            //TODO: create and change to "DEFAULT_EA_TICK_DELAY"
            uint32_t SelectedTickDelay = 500e3;
            uint8_t SelectedMaxThreads = 1;

            ThreadID PreviewThreadID = 0;
        } m_Settings;

        std::unordered_map<ThreadID, Application::GameRunner *> m_GameRunnerThreadMap;
        Application::EventListener m_EventListener;

    public:
        void FlushSettings(EAController&);
        

        void UpdateGameRunner(ThreadID threadID, Application::GameRunner *gameRunner)
        {
            auto findIt = m_GameRunnerThreadMap.find(threadID);
            if (findIt == m_GameRunnerThreadMap.end())
                m_GameRunnerThreadMap.insert({threadID, gameRunner});
            else
                findIt->second = gameRunner;

            m_EventListener.On<EH_GameRunnerChanged>({threadID, gameRunner});
        }

        //Danger: nullable pointer
        Application::GameRunner *GetGameRunner(ThreadID threadID) const
        {
            auto findIt = m_GameRunnerThreadMap.find(threadID);
            if (findIt == m_GameRunnerThreadMap.end())
                return nullptr;
            else
                return findIt->second;
        }

        inline void RegisterEAGameRunnerChanged(EH_GameRunnerChanged *eHandler) { m_EventListener.Register(eHandler); }
        inline void UnregisterEAGameRunnerChanged(EH_GameRunnerChanged *eHandler) { m_EventListener.Unregister(eHandler); }

        friend class Application::EAGameGuiLayer;
    };
} // namespace EvoAlg
