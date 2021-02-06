#include "ThreadLoop.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "mypch.hpp"

//TODO: turn off for more performance
static const bool JOIN_ABANDONED_THREADS = true;

#define TLL_ENABLED 0
#if TLL_ENABLED == 1
#define TLL(MACRO, ...) MACRO(__VA_ARGS__)
#else
#define TLL(MACRO, ...)
#endif

namespace Application
{
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
        m_TickDelay = nullptr;
        delete m_EventListener;
    }

    void ThreadLoop::InnerLoop()
    {
        //Assuming m_StateMutex is locked from Start() function

        {
            std::lock_guard<std::mutex> guard(m_StateMutex);

            DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::InnerLoop) Trying to start a ThreadLoop while it's in an invalid state");

            if (m_State != State::INACTIVE)
                return;

            m_State = State::RUNNING;

            m_EventListener->On<EH_ThreadStarted>();
        }

        uint32_t currTickDelay = 1;
        while (true)
        {
            //Allow state to be changed by other threads...
            {
                std::lock_guard<std::mutex> guard(m_StateMutex);

                if (m_State != State::RUNNING)
                    break;

                if (m_TickDelay != nullptr)
                    currTickDelay = *m_TickDelay;

                DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::InnerLoop) m_AliveCheckFunction is nullptr!!");
                DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::InnerLoop) m_TickFunction is nullptr!!");

                if (!m_Paused)
                {
                    if (!m_AliveCheckFunction()) //If finished naturally (neither force nor abortion)
                        m_State = State::FINISHED;
                    else
                        m_TickFunction();
                }
            }

            usleep(currTickDelay);
        }

        {
            std::lock_guard<std::mutex> guard(m_StateMutex);

            DE_ASSERT(m_State != State::RUNNING, "(ThreadLoop::InnerLoop) Invalid Running state after while m_State == State::RUNNING");
            DE_ASSERT(m_State != State::INACTIVE, "(ThreadLoop::InnerLoop) Invalid Inactive state after while m_State == State::RUNNING");

            if (m_State == State::FORCED_STOP)
                m_EventListener->OnAsync<EH_ThreadEnded>(ThreadEndedReason::FORCED_STOP);
            else if (m_State == State::FINISHED)
                m_EventListener->OnAsync<EH_ThreadEnded>(ThreadEndedReason::FINISHED);
            else if (m_State == State::ABANDONED)
            {
                DE_DEBUG("(ThreadLoop[{0}] inner) Ignoring abandoned thread", m_DebugName);
            }

            m_State = State::INACTIVE;
        }
    }

    void ThreadLoop::Pause(bool paused)
    {
        DE_ASSERT(m_Paused != paused, "(ThreadLoop) Invalid Pause/Unpause");
        m_Paused = paused;
    }

    void ThreadLoop::Start(const uint32_t *tickDelay)
    {
        DE_ASSERT(m_Thread == nullptr, "This is a temporary assert, maybe you really want to start a ThreadLoop twice");

        DE_ASSERT(m_AliveCheckFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_AliveCheckFunction == nullptr!!");
        DE_ASSERT(m_TickFunction != nullptr, "(ThreadLoop::Start) Trying to start a ThreadLoop with m_TickFunction == nullptr!!");
        DE_ASSERT(m_State == State::INACTIVE, "(ThreadLoop::Start) Trying to start a ThreadLoop while it's already running!!!")

        {
            std::lock_guard<std::mutex> guard(m_StateMutex);
            m_TickDelay = tickDelay;
            m_Paused = false;
        }

        if (m_Thread != nullptr)
            delete m_Thread;
        m_Thread = new std::thread(std::bind(&ThreadLoop::InnerLoop, this));
    }

    void ThreadLoop::Stop()
    {
        {
            std::lock_guard<std::mutex> guard(m_StateMutex);
            DE_ASSERT(m_State == State::RUNNING, "(ThreadLoop::Stop) Trying to stop a ThreadLoop while it's NOT running!!!")
            m_State = State::FORCED_STOP;
        }
    }

    void ThreadLoop::Abandon()
    {
        bool isRunning = false;

        {
            std::lock_guard<std::mutex> guard(m_StateMutex);
            isRunning = m_State == State::RUNNING;
            m_EventListener->Clear();

            m_State = State::ABANDONED;
            m_AliveCheckFunction = nullptr;
            m_TickFunction = nullptr;
            TLL(DE_DEBUG, "(ThreadLoop[{0}] Abandon) Marking as ABANDONED", m_DebugName);
        }

        if (JOIN_ABANDONED_THREADS && m_Thread != nullptr && m_Thread->joinable() && isRunning)
        {
            DE_DEBUG("Joining threadloop..... @ThreadLoop::Abandon");
            m_Thread->join();
            DE_DEBUG("Thread ended normally @ThreadLoop::Abandon");
        }
    }

} // namespace Application
