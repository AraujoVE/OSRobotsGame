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
        int id;
        time_t lastUpdateTime;
        int timeUnits;
        int predictedTime;
        int efficiency;

    public:
        static int lastId;
        Task(RobotFunctions funct);
        void initializeParameters(RobotFunctions funct);
        ~Task();

        RobotFunctions getType() const;
        int getRobotsNo() const;
        int getPredictedTime() const;
        int getId() const;
        
        void setType(RobotFunctions newType);
        void setPredictedTime(int newPredictedTime);
        void setRobotsNo(int newRobotsNo);
        
        void efficiencyUpdate(int newEfficiency);
        bool updatePredictedTime(time_t curTime,int newEfficiency,int newRobotsNo);
        void createThread();
        void deleteThread();
};
#endif