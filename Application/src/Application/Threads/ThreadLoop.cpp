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
    ThreadLoop::ThreadLoop(const std::string &debugName)
        : m_State(State::INACTIVE), m_Paused(false),
          m_EventListener(new EventListener()),
          m_DebugName(debugName)
    {
    }

    ThreadLoop::~ThreadLoop()
    {
        delete m_Thread;
        delete m_ExecutionParams;
    }

    void ThreadLoop::Loop()
    {
        {
            std::lock_guard<std::mutex> stateGuard(m_StateMutex);
            DE_ASSERT(m_State == State::STARTING);
            m_State = State::RUNNING;
        }

        m_cvFullyStarted.notify_one();

        bool finished = false;
        while (m_State == State::RUNNING)
        {
            if (!m_Paused)
            {
                std::lock_guard<std::mutex> stateGuard(m_StateMutex);
                m_ExecutionParams->m_TickFunction();
                if (!m_ExecutionParams->m_AliveCheckFunction())
                {
                    finished = true;
                    break;
                }
            }

            usleep(m_ExecutionParams->TickDelay);
        }
        State endState = m_State;
        m_State = State::INACTIVE;

        if (endState == State::STOPPING_ABANDONED)
            return;

        if (finished)
            m_EventListener->OnAsync<EH_ThreadEnded>(ThreadEndedReason::FINISHED);
        else
            m_EventListener->OnAsync<EH_ThreadEnded>(ThreadEndedReason::FORCED_STOP);
    }

    void ThreadLoop::Pause(bool paused)
    {
        m_Paused = paused;
    }

    void ThreadLoop::Start(ThreadLoopParams *params)
    {
        {
            std::lock_guard<std::mutex> stateGuard(m_StateMutex);

            DE_ASSERT(params != nullptr);
            DE_ASSERT(m_ExecutionParams == nullptr);
            DE_ASSERT(m_State == State::INACTIVE);
            m_State = State::STARTING;
        }

        m_ExecutionParams = params;
        m_Thread = new std::thread(std::bind(&ThreadLoop::Loop, this));

        {
            std::unique_lock<std::mutex> stateGuard(m_StateMutex);
            m_cvFullyStarted.wait(stateGuard, [this]{ return m_State == State::RUNNING; });
        }
    }

    void ThreadLoop::Stop()
    {

        {
            std::lock_guard<std::mutex> stateGuard(m_StateMutex);
            DE_ASSERT(m_State <= State::RUNNING);
            m_State = State::STOPPING_ABANDONED;
        }
        if (m_Thread != nullptr && m_Thread->joinable())
            m_Thread->join();
    }

    void ThreadLoop::Abandon()
    {
        {
            std::lock_guard<std::mutex> stateGuard(m_StateMutex);
            DE_ASSERT(m_State <= State::RUNNING);
            m_State = State::STOPPING_ABANDONED;
            if (m_Thread != nullptr && m_Thread->joinable())
                m_Thread->detach();
        }
    }

} // namespace Application
