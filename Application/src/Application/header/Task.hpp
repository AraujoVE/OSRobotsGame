#ifndef TASK
#define TASK

#include "Application/header/RobotFunctions.hpp"
#include "Application/Threads/ThreadLoop.hpp"

#include <memory>

namespace Application
{
    class EventListener;

    class Task final
    {
    public:
        Task(RobotFunction funct);
        ~Task();
        Task(Task &&) = default;

        using TaskID = unsigned long int;

        inline TaskID GetID() const { return id; }
        inline RobotFunction GetRobotFunction() const { return function; }

        inline int GetRobotsNo() const { return robotsNo; }
        inline int GetCurProgress() const { return curProgress; }
        inline int GetProgressLength() const { return progressLength; }

        inline time_t GetRemainingTime() const { return remainingTime; }
        inline float GetGainedGoods() const { return gainedGoods; }

        inline int GetAvgReward() const { return AVG_REWARD; }
        inline time_t GetLastUpdate() const { return lastUpdateTime; }
    private:
        std::unique_ptr<EventListener> m_EventListener;

        int TIME_STEP;
        const int INIT_TIME_STEP;
        const int MAX_TIME_STEPS;
        const int MIN_REWARD;
        const int REWARD_RANGE;
        const float FAILURE_TAX;
        const int AVG_REWARD = (int)(((float)TIME_STEP) * ((float)INIT_TIME_STEP + ((float)MAX_TIME_STEPS - 1.0) / 2.0) * ((float)MIN_REWARD + ((float)REWARD_RANGE - 1.0) / 2.0));

        static TaskID s_NextID;

        ThreadLoop m_ThreadLoop;
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
#endif