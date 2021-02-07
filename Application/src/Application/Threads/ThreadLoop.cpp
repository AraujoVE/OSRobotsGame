#include "ThreadLoop.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

#define TLL_ENABLED 0
#if TLL_ENABLED == 1
#define TLL(MACRO, ...) MACRO(__VA_ARGS__)
#else
#define TLL(MACRO, ...)
#endif

namespace Application
{
    void *threadRountine(void *threadLoopV)
    {
        auto *threadLoop = (ThreadLoop *)threadLoopV;
        threadLoop->m_EventListener->On<EH_ThreadStarted>();
        threadLoop->InnerLoop();
        return NULL;
    }

    const uint32_t ThreadLoop::s_HumanTickDelay = HUMAN_TICK_DELAY_MICRO;
    ThreadLoop::ThreadLoop(const std::string &debugName)
        : m_TickFunction(nullptr), m_AliveCheckFunction(nullptr),
          m_State(State::INACTIVE), m_Paused(false),
          m_TickDelay(&s_HumanTickDelay),
          m_EventListener(new EventListener()),
          m_DebugName(debugName)
    {
    }

    ThreadLoop::~ThreadLoop()
    {
        Abandon();
        m_InnerLoopMutex.Lock(); //Waits for innerloop to end
        {
            m_TickDelay = nullptr;
            delete m_EventListener;
        }
        m_InnerLoopMutex.Unlock();
        delete m_Thread;
    }

    void ThreadLoop::InnerLoop()
    {
        m_InnerLoopMutex.Lock();
        DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::InnerLoop) Trying to start a ThreadLoop while it's already running!!!");

        m_State = State::RUNNING;
        while (m_State == State::RUNNING)
        {
            TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Start of loop", m_DebugName);

            if (!m_Paused)
            {
                TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Not paused, checking if abandoned...", m_DebugName);

                m_FunctsMutex.Lock();
                if (m_State == State::ABANDONED)
                {
                    m_FunctsMutex.Unlock();
                    TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Abandoned!! breaking execution loop", m_DebugName);
                    break;
                }

                TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Checking for nullptrs", m_DebugName);
                DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::InnerLoop) m_AliveCheckFunction is nullptr!!");
                DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::InnerLoop) m_TickFunction is nullptr!!");

                TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Checking if still alive", m_DebugName);
                if (!m_AliveCheckFunction())
                {
                    TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) ThreadLoop MARKED AS NOT ALIVE, leaving execution loop", m_DebugName);
                    m_State = State::FINISHED;
                    m_FunctsMutex.Unlock();
                    break;
                }

                TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Calling tick...", m_DebugName);
                m_TickFunction();
                m_FunctsMutex.Unlock();
            }
            else
            {
                TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Ignoring paused tick...", m_DebugName);
            }

            TLL(DE_DEBUG, "(ThreadLoop[{0}] inner) Repeating loop...", m_DebugName);

            DE_ASSERT(m_TickDelay != nullptr);
            //TODO: remove if break if no problems
            if (m_TickDelay == nullptr)
            {
                break;
            }
            usleep(*m_TickDelay);
        }

        DE_ASSERT(m_State != State::RUNNING, "(ThreadLoop::InnerLoop) Invalid Running state after while m_State == State::RUNNING");
        DE_ASSERT(m_State != State::INACTIVE, "(ThreadLoop::InnerLoop) Invalid Inactive state after while m_State == State::RUNNING");

        if (m_State == State::FORCED_STOP)
            m_EventListener->On<EH_ThreadEnded>(ThreadEndedReason::FORCED_STOP);
        else if (m_State == State::FINISHED)
            m_EventListener->On<EH_ThreadEnded>(ThreadEndedReason::FINISHED);
        else if (m_State == State::ABANDONED)
        {
            DE_DEBUG("(ThreadLoop[{0}] inner) Ignoring abandoned thread", m_DebugName);
        }

        m_State = State::INACTIVE;
        m_InnerLoopMutex.Unlock();
    }

    void ThreadLoop::Pause(bool paused)
    {
        DE_ASSERT(m_Paused != paused, "(ThreadLoop) Invalid Pause/Unpause");
        m_Paused = paused;
    }

    void ThreadLoop::Start(const uint32_t *tickDelay)
    {
        DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_AliveCheckFunction == nullptr!!");
        DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_TickFunction == nullptr!!");
        DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::Start) Trying to start a ThreadLoop while it's already running!!!")

        m_TickDelay = tickDelay;

        m_Paused = false;

        m_Thread = new std::thread(std::bind(&threadRountine, this));
    }

    void ThreadLoop::Stop()
    {
        DE_ASSERT(m_State == State::RUNNING, "(ThreadLoop::Stop) Trying to stop a ThreadLoop while it's NOT running!!!")
        m_State = State::FORCED_STOP;
    }

    void ThreadLoop::Abandon()
    {
        m_FunctsMutex.Lock();
        {
            m_EventListener->Clear();

            m_AliveCheckFunction = nullptr;
            m_TickFunction = nullptr;
            TLL(DE_DEBUG, "(ThreadLoop[{0}] Abandon) Marking as ABANDONED", m_DebugName);
            m_State = State::ABANDONED;
        }
        m_FunctsMutex.Unlock();
    }

} // namespace Application
