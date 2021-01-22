#include "ThreadLoop.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

namespace Application
{
    void *threadRountine(void *threadLoopV)
    {
        auto *threadLoop = (ThreadLoop *)threadLoopV;
        threadLoop->m_EventListener->On<EH_ThreadStarted>();
        threadLoop->InnerLoop();
        return NULL;
    }

    ThreadLoop::ThreadLoop(const std::string &debugName)
        : m_TickFunction(nullptr), m_AliveCheckFunction(nullptr),
          m_State(State::INACTIVE), m_Paused(false),
          m_EventListener(new EventListener()),
          m_DebugName(debugName)
    {
    }

    ThreadLoop::~ThreadLoop()
    {
        m_EventListener->Clear();
        delete m_EventListener;
    }

    void ThreadLoop::InnerLoop()
    {
        DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::InnerLoop) Trying to start a ThreadLoop while it's already running!!!");

        m_State = State::RUNNING;
        while (m_State == State::RUNNING)
        {
            DE_DEBUG("(ThreadLoop::InnerLoop) Start of loop");

            if (!m_Paused)
            {
                DE_DEBUG("(ThreadLoop::InnerLoop) Not paused, checking if abandoned...");

                m_FunctsMutex.Lock();
                if (m_State == State::ABANDONED)
                {
                    m_FunctsMutex.Unlock();
                    DE_DEBUG("(ThreadLoop::InnerLoop) Abandoned!! breaking execution loop");
                    break;
                }

                DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::InnerLoop::InnerLoop) m_AliveCheckFunction is nullptr!!");
                DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::InnerLoop::InnerLoop) m_TickFunction is nullptr!!");

                DE_DEBUG("(ThreadLoop::InnerLoop) Checking if still alive");
                if (!m_AliveCheckFunction())
                {
                    DE_DEBUG("(ThreadLoop::InnerLoop) ThreadLoop MARKED AS NOT ALIVE, leaving execution loop");
                    m_State = State::FINISHED;
                    m_FunctsMutex.Unlock();
                    break;
                }

                DE_DEBUG("(ThreadLoop::InnerLoop) Calling tick...");
                m_TickFunction();
                m_FunctsMutex.Unlock();
            }
            else
            {
                DE_DEBUG("(ThreadLoop::InnerLoop) Ignoring paused tick...");
            }

            DE_DEBUG("(ThreadLoop::InnerLoop) Repeating loop...");

            usleep(m_TickDelay);
        }

        DE_ASSERT(m_State != State::RUNNING, "(ThreadLoop::InnerLoop) Invalid Running state after while m_State == State::RUNNING");
        DE_ASSERT(m_State != State::INACTIVE, "(ThreadLoop::InnerLoop) Invalid Inactive state after while m_State == State::RUNNING");

        if (m_State == State::FORCED_STOP)
            m_EventListener->On<EH_ThreadEnded>(ThreadEndedReason::FORCED_STOP);
        else if (m_State == State::FINISHED)
            m_EventListener->On<EH_ThreadEnded>(ThreadEndedReason::FINISHED);
        else if (m_State == State::ABANDONED)
            DE_TRACE("(ThreadLoop) Ignoring abandoned thread");

        m_State = State::INACTIVE;
    }

    void ThreadLoop::Pause(bool paused)
    {
        DE_ASSERT(m_Paused != paused, "(ThreadLoop) Invalid Pause/Unpause");
        m_Paused = paused;
    }

    void ThreadLoop::Start(uint32_t tickDelay)
    {
        DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_AliveCheckFunction == nullptr!!");
        DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_TickFunction == nullptr!!");
        DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::Start) Trying to start a ThreadLoop while it's already running!!!")

        m_TickDelay = tickDelay;

        m_Paused = false;
        pthread_create(&m_Thread, NULL, &threadRountine, this);
    }

    void ThreadLoop::Stop()
    {
        DE_ASSERT(m_State == State::RUNNING, "(ThreadLoop::Stop) Trying to stop a ThreadLoop while it's NOT running!!!")
        m_State = State::FORCED_STOP;
    }

    void ThreadLoop::Abandon()
    {
        DE_ASSERT(m_State == State::RUNNING, "(ThreadLoop::Abandon) Trying to abandon a ThreadLoop while it's NOT running!!!")
        m_FunctsMutex.Lock();
        {
            m_EventListener->Clear();

            m_AliveCheckFunction = nullptr;
            m_TickFunction = nullptr;
            m_State = State::ABANDONED;
        }
        m_FunctsMutex.Unlock();
    }

} // namespace Application
