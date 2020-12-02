#ifndef TASK
#define TASK

#include "RobotFunctions.hpp"
// #include "RobotsManagement.hpp"
// #include "VillageStats.hpp"
// #include <vector>
//#include <map> 
#include <ctime>
//#include <iostream>

class Task{
    private:
        const int TIME_STEP = 30;
        const int INIT_TIME_STEP = 2;


        RobotFunctions type;
        int robotsNo;
        time_t initTime;
        time_t lastUpdateTime;
        int timeUnits;
        int predictedTime;
        int efficiency;

    public:
        Task(RobotFunctions funct,time_t id);
        void initializeParameters(RobotFunctions funct,time_t id);
        ~Task();

        RobotFunctions getType() const;
        int getRobotsNo() const;
        time_t getInitTime() const;
        int getPredictedTime() const;
        
        void setType(RobotFunctions newType);
        void setInitTime(time_t newInitTime);
        void setPredictedTime(int newPredictedTime);
        void setRobotsNo(int newRobotsNo);
        
        void efficiencyUpdate(int newEfficiency);
        bool updatePredictedTime(time_t curTime,int newEfficiency,int newRobotsNo);
        void createThread();
        void deleteThread();
};
#endif