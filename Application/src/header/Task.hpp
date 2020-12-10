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

        static int lastId;
        RobotFunctions::RobotFunction type;
        int robotsNo;
        int id;
        time_t lastUpdateTime;
        int timeUnits;
        int predictedTime;
        time_t initTime; // time when task started
        int efficiency;

    public:
        Task(RobotFunctions::RobotFunction funct);
        void initializeParameters(RobotFunctions::RobotFunction funct);
        ~Task();

        RobotFunctions::RobotFunction getType() const;
        int getRobotsNo() const;
        int getPredictedTime() const;
        int getId() const;
        time_t getInitTime() const;
        
        void setType(RobotFunctions::RobotFunction newType);
        void setPredictedTime(int newPredictedTime);
        void setRobotsNo(int newRobotsNo);
        
        void efficiencyUpdate(int newEfficiency);
        bool updatePredictedTime(int newEfficiency,int newRobotsNo);
        void createThread();
        void deleteThread();
};
#endif