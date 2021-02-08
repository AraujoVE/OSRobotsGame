#pragma once

#include "Application/Game/GameConsts.hpp"

#include <functional>
#include <memory>
#include <thread>
#include <mutex>

namespace Application
{
    class EventListener;
    namespace ThreadEndedReason
    {
        enum ThreadEndedReason_t
        {
            FORCED_STOP,
            FINISHED
        };
    }

    

    class ThreadLoop final
    {
    public:

        enum class State {
            INACTIVE, STARTING, RUNNING, FORCED_STOP, FINISHED, ABANDONED
        };

        using TickFunction = std::function<void()>;
        using AliveCheckFunction = std::function<bool()>;


    private:
        std::thread *m_Thread = nullptr;

        std::function<void()> m_TickFunction;
        std::function<bool()> m_AliveCheckFunction;
        
        void InnerLoop();
        friend void *threadRountine(void *threadLoopV);

        std::mutex m_StateMutex;

        State m_State;
        bool m_Paused;

        const static uint32_t s_HumanTickDelay;
        const uint32_t *m_TickDelay;
    public:
        EventListener *m_EventListener;

        std::string m_DebugName;
        ThreadLoop(const std::string& debugName);
        ~ThreadLoop();

        inline void SetTickFunction(TickFunction &&func) { m_TickFunction = func; }
        inline void SetAliveCheckFunction(AliveCheckFunction &&func) { m_AliveCheckFunction = func; }

        inline ThreadLoop::State GetState() { return m_State; }

        void Pause(bool paused = true);
        inline void Unpause() { Pause(false); }
        inline bool IsPaused() const { return m_Paused; }
        inline bool IsRunning() const { return m_State == State::RUNNING; }
        void Start(const uint32_t *tickDelay);
        void Stop();
        void Abandon();
    };

} // namespace Application
