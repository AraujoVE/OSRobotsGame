#pragma once

#include "Types.hpp"
#include "Application/Game/Types.fwd.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include <unordered_map>
#include <optional>

namespace EvoAlg
{
    class EAGuiProps
    {
        std::unordered_map<ThreadID, Application::GameRunner *> m_GameRunnerThreadMap;
        Application::EventListener m_EventListener;

    public:
        void UpdateGameRunner(ThreadID threadID, Application::GameRunner *gameRunner)
        {
            auto findIt = m_GameRunnerThreadMap.find(threadID);
            if (findIt == m_GameRunnerThreadMap.end())
                m_GameRunnerThreadMap.insert({threadID, gameRunner});
            else
                findIt->second = gameRunner;

            m_EventListener.On<EH_EAGameRunnerChanged>({threadID, gameRunner});
        }

        //Danger: nullable pointer
        GameRunner *GetGameRunner(ThreadID threadID) const
        {
            auto findIt = m_GameRunnerThreadMap.find(threadID);
            if (findIt == m_GameRunnerThreadMap.end())
                return nullptr;
            else return findIt->second;
        }
    };
} // namespace EvoAlg
