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

    using TickFunction = std::function<void()>;
    using AliveCheckFunction = std::function<bool()>;
    struct ThreadLoopParams
    {
    private:
        TickFunction m_TickFunction;
        AliveCheckFunction m_AliveCheckFunction;

    public:
        uint32_t TickDelay;

        ThreadLoopParams(TickFunction tickFn, AliveCheckFunction aliveFn, uint32_t tickDelay = DEFAULT_GAME_TICK_DELAY)
            : m_TickFunction(tickFn), m_AliveCheckFunction(aliveFn), TickDelay(tickDelay) {}

        friend class ThreadLoop;
    };

    class ThreadLoop final
    {
    public:
        enum class State
        {
            INACTIVE,
            STARTING,
            RUNNING,
            STOPPING_FORCED,
            STOPPING_ABANDONED,
        };

    private:
        std::condition_variable m_cvFullyStarted;
        std::thread *m_Thread = nullptr;
        std::mutex m_StateMutex;

        ThreadLoopParams *m_ExecutionParams = nullptr;

        State m_State;
        bool m_Paused;

        void Loop();
    public:
        EventListener *m_EventListener;
        std::string m_DebugName;

        ThreadLoop(const std::string &debugName);
        ~ThreadLoop();

        inline ThreadLoop::State GetState() { return m_State; }

        void Start(ThreadLoopParams* params);

        void Pause(bool paused = true);
        inline void Unpause() { Pause(false); }
        inline bool IsPaused() const { return m_Paused; }
        inline bool IsRunning() const { return m_State == State::RUNNING; }

        void Stop();
        void Abandon();
    };

} // namespace Application
