#ifndef TASK
#define TASK

#include "RobotFunctions.hpp"
// #include "RobotsManagement.hpp"
// #include "VillageStats.hpp"
// #include <vector>
//#include <map>
#include <ctime>
//#include <iostream>

namespace Application
{
    class Task
    {
    private:
        const static int TIME_STEP = 10;
        const static int INIT_TIME_STEP = 2;
        const static int MAX_TIME_STEPS = 10;
        const static int MIN_REWARD = 2;
        const static int REWARD_RANGE = 3;
        constexpr static float FAILURE_TAX = 0.8;

        static int lastId;
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

    public:
        Task(RobotFunction funct);
        void initializeParameters(RobotFunction funct);
        ~Task();

        int getId() const;
        RobotFunction getType() const;
        int getRobotsNo() const;
        int getProgressLength() const;
        int getCurProgress() const;
        time_t getLastUpdate() const;
        time_t getRemainingTime() const;
        float getGainedGoods() const;

        void setRobotsNo(int newRobotsNo);
        bool updateTask();

        int calcLostRobots();

        void createThread();
        void deleteThread();
    };

} // namespace Application
#endif