#pragma once

#include "cpphack.hpp"

#include "Application/Game/Ingame/RobotFunctions.hpp"
#include "Application/Threads/ThreadLoop.hpp"
#include "Application/Game/GameConsts.hpp"

#include <memory>

namespace Application
{
    class EventListener;

    class Task final
    {
    public:
        using TaskID = unsigned long int;

    private:
        const GameConstsCache &m_GameConstsCache;

        Task(RobotFunction funct, GameConstsCache &gameConsts, Task::TaskID id);

    public:
        ~Task();
        Task(Task &&) = default;

        inline TaskID GetID() const { return id; }
        inline RobotFunction GetRobotFunction() const { return function; }

        inline int GetRobotsNo() const { return robotsNo; }
        inline int GetCurProgress() const { return curProgress; }
        inline int GetProgressLength() const { return progressLength; }

        inline time_t GetRemainingTime() const { return remainingTime; }
        inline float GetGainedGoods() const { return gainedGoods; }

        inline int GetAvgReward() const { return m_GameConstsCache.AVG_REWARD; }
        inline time_t GetLastUpdate() const { return lastUpdateTime; }

    private:
        std::unique_ptr<EventListener> m_EventListener;

        ThreadLoop *m_ThreadLoop;
        bool m_Running;

        const int id;
        const RobotFunction function;
        int robotsNo;
        int progressLength;
        int curProgress;
        time_t lastUpdateTime;
        time_t remainingTime;
        int timeUnits;
        int predictedTime;
        int rewardMultiplier;
        float gainedGoods;

    private:
        void setRobotsNo(int newRobotsNo);
        void UpdateTask();
        inline bool IsTaskCompleted() { return remainingTime == 0; }

        int CalcLostRobots();

        void Start();
        void Cancel();
        void MarkAsIgnored();

        bool OnThreadLoopStarted();
        bool OnThreadLoopEnded(ThreadEndedReason::ThreadEndedReason_t reason);

        friend class RobotsManagement;
    };
} // namespace Application