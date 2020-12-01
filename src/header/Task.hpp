#include "header/RobotsManagement.hpp"
#include "header/VillageStats.hpp"
//#include <vector>
//#include <map> 
//#include <ctime>
//#include <iostream>

class Task{
    public:
        const int TIME_STEP = 30;
        const int INIT_TIME_STEP = 2;


        RobotFunctions type;
        int robotsNo;
        time_t initTime;
        int remainingTime;
        int *efficiency;

    private:
        Task(RobotFunctions funct,time_t id,int *efficiency);
        void Task::initializeParameters(RobotFunctions funct,time_t id,int *newEfficiency);
        ~Task();

        RobotFunctions getType();
        int getRobotsNo();
        time_t getInitTime();
        int getRemainingTime();

        void setType(RobotFunctions newType);
        void setRobotsNo(int newRobotsNo);
        void setInitTime(time_t newInitTime);
        void setRemainingTime(int newRemainingTime);

        void updateRemainingTime();

        void createThread();
        void deleteThread();
};