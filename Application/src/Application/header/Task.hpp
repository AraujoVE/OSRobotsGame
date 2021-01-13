#ifndef TASK
#define TASK

#include "RobotFunctions.hpp"
#include <functional>

namespace Application
{
    using TaskID = unsigned long int;

    class Task
    {
    public:
        using OnFinishedCallback = std::function<void(Task&)>;

    private:
        const int TIME_STEP;
        const int INIT_TIME_STEP;
        const int MAX_TIME_STEPS;
        const int MIN_REWARD;
        const int REWARD_RANGE;
        const float FAILURE_TAX;
        const int AVG_REWARD = (int)(((float)TIME_STEP)*((float)INIT_TIME_STEP + ((float)MAX_TIME_STEPS - 1.0)/2.0)*((float)MIN_REWARD + ((float)REWARD_RANGE - 1.0)/2.0));

        static TaskID s_NextID;

        pthread_t m_TaskThread;
        bool m_Running;
            
        int id;
        RobotFunction type;
        int robotsNo;
        int progressLength;
        int curProgress;
        time_t lastUpdateTime;
        time_t remainingTime;
        int timeUnits;
        int predictedTime;
        int rewardMultiplier;
        float gainedGoods;

        OnFinishedCallback m_OnFinishedCallback;

    public:

        Task(RobotFunction funct, OnFinishedCallback onFinishedCallback);
        void initializeParameters(RobotFunction funct);
        ~Task();
        Task(Task&&) = default;

        TaskID getId() const;
        RobotFunction getType() const;
        int getRobotsNo() const;
        int getProgressLength() const;
        int getCurProgress() const;
        time_t getLastUpdate() const;
        time_t getRemainingTime() const;
        float getGainedGoods() const;
        int getAvgReward() const;

        void setRobotsNo(int newRobotsNo);
        bool updateTask();

        int calcLostRobots();

        void threadLoop();
        void start();
        void stop();

        void detach();
    };

    void *runThreadLoop(void *taskObject);
} // namespace Application
#endif