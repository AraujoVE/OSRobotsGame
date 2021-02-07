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
        while (m_Running && m_AliveCheckFunction())
        {
            m_TickFunction();
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
    
    void ThreadLoop::Stop() { m_Running = false; }
} // namespace Application
