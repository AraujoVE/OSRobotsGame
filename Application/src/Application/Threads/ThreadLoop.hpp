#pragma once

#include "DampEngine/Threads/Mutex.hpp"

#include <functional>
#include <memory>
#include <pthread.h>

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
            INACTIVE, RUNNING, FORCED_STOP, FINISHED, ABANDONED
        };

        using TickFunction = std::function<void()>;
        using AliveCheckFunction = std::function<bool()>;


    private:
        pthread_t m_Thread;

        std::function<void()> m_TickFunction;
        std::function<bool()> m_AliveCheckFunction;
        
        void InnerLoop();
        friend void *threadRountine(void *threadLoopV);

        DampEngine::Mutex m_FunctsMutex;

        State m_State;
        bool m_Paused;
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
        inline bool IsPaused() { return m_Paused; }
        void Start();
        void Stop();
        void Abandon();
    };

} // namespace Application
