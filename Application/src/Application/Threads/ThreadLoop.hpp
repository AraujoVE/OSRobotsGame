#pragma once

#include "Application/Events/EventListener.hpp"

#include <functional>
#include <memory>
#include <pthread.h>

namespace Application
{
    namespace ThreadEndedReason
    {
        enum ThreadEndedReason_t
        {
            STOP,
            ALIVE_CHECK
        };
    }

    DA_EVENT(EH_ThreadStarted, bool(void));
    DA_EVENT(EH_ThreadEnded, bool(ThreadEndedReason::ThreadEndedReason_t endReason));

    void *threadRountine(void *threadLoopV);

    class ThreadLoop
    {
    private:
        pthread_t m_Thread;

        std::function<void()> m_TickFunction;
        std::function<bool()> m_AliveCheckFunction;
        bool m_Running;

        void InnerLoop()
        {
            while (m_Running && m_AliveCheckFunction())
            {
                m_TickFunction();
            }
            bool forcedStop = !m_Running;
            m_Running = false;
            m_EventListener.On<EH_ThreadEnded>(forcedStop ? ThreadEndedReason::STOP : ThreadEndedReason::ALIVE_CHECK);
        }

        friend void *threadRountine(void *threadLoopV);
        const static std::function<bool()> s_DefaultAliveCheckFunction;

    public:
        EventListener m_EventListener;

        ThreadLoop(
            std::function<void()> loopFunction,
            std::function<bool()> aliveCheckFunction = s_DefaultAliveCheckFunction)
            : m_TickFunction(loopFunction), m_AliveCheckFunction(aliveCheckFunction)
        {
        }

        void Start()
        {
            m_Running = true;
            pthread_create(&m_Thread, NULL, &threadRountine, this);
            m_EventListener.On<EH_ThreadStarted>();
        }
        inline void Stop() { m_Running = false; }
    };
    
} // namespace Application
