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
            STOP,
            FINISHED
        };
    }

    class ThreadLoop
    {
    private:
        pthread_t m_Thread;

        std::function<void()> m_TickFunction;
        std::function<bool()> m_AliveCheckFunction;
        bool m_Running;

        void InnerLoop();
        friend void *threadRountine(void *threadLoopV);
        const static std::function<bool()> s_DefaultAliveCheckFunction;
        const static std::function<bool()> s_StoppedAliveCheckFunction;

        DampEngine::Mutex m_FunctsMutex;
    public:
        std::unique_ptr<EventListener> m_EventListener;

        ThreadLoop(
            std::function<void()> loopFunction,
            std::function<bool()> aliveCheckFunction = s_DefaultAliveCheckFunction);

        void Start();
        void Stop();
        void Abandon();
    };

} // namespace Application
