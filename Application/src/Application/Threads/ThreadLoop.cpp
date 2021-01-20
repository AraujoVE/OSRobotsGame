#include "ThreadLoop.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

namespace Application
{
    void *threadRountine(void *threadLoopV)
    {
        auto *threadLoop = (ThreadLoop *)threadLoopV;
        threadLoop->m_EventListener->On<EH_ThreadStarted>();
        threadLoop->InnerLoop();
        return NULL;
    }

    const std::function<bool()> ThreadLoop::s_DefaultAliveCheckFunction = []() { return true; };

    //TODO: make member of class
    const std::function<bool()> ThreadLoop::s_StoppedAliveCheckFunction = []() { return true; };

    ThreadLoop::ThreadLoop(
        std::function<void()> loopFunction,
        std::function<bool()> aliveCheckFunction)
        : m_TickFunction(loopFunction),
          m_AliveCheckFunction(aliveCheckFunction),
          m_EventListener(new EventListener())
    {
    }

    void ThreadLoop::InnerLoop()
    {
        while (m_Running)
        {


            m_FunctsMutex.Lock();
            {
                if (!m_AliveCheckFunction()) {
                    m_FunctsMutex.Unlock();
                    break;
                }
                m_TickFunction();
            }
            m_FunctsMutex.Unlock();
        }
        bool forcedStop = !m_Running;
        m_Running = false;
        m_EventListener->On<EH_ThreadEnded>(forcedStop ? ThreadEndedReason::STOP : ThreadEndedReason::FINISHED);
    }

    void ThreadLoop::Start()
    {
        m_Running = true;
        pthread_create(&m_Thread, NULL, &threadRountine, this);
    }

    void ThreadLoop::Stop()
    {
        m_Running = false;
    }

    void ThreadLoop::Abandon()
    {
        m_FunctsMutex.Lock();
        {
            m_Running = false;
            m_AliveCheckFunction = s_StoppedAliveCheckFunction;
            m_TickFunction = []() {};
        }
        m_FunctsMutex.Unlock();
    }

} // namespace Application
    