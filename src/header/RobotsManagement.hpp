#ifndef ROBOTS_MANAGEMENT
#define ROBOTS_MANAGEMENT

#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include <vector>
#include <map> 

enum RobotFunctions{
    PROTECTION,
    RESOURCE_GATHERING,
    HUNT,
    MEDICINE,
    CONSTRUCTION,
};

class RobotsManagement{

    private:
        int totRobots;
        int freeRobots;
        int efficiency;
        int prodCost;
        VillageStats *villageStats;
        std::vector<std::map<time_t,Task>> *tasks;

    public:
        RobotsManagement();
        void initializeStats();
        ~RobotsManagement();

        int RobotsManagement::getTotRobots();
        int RobotsManagement::getFreeRobots();
        int RobotsManagement::getEfficiency();
        int RobotsManagement::getProdCost();

        void RobotsManagement::setTotRobots(int newTotRobots);
        void RobotsManagement::setFreeRobots(int newFreeRobots);
        void RobotsManagement::setEfficiency(int newEfficiency);
        void RobotsManagement::setProdCost(int newProdCost);
        void RobotsManagement::setVillageStats(VillageStats *newVillageStats);
        void RobotsManagement::setTasks(std::vector<std::map<time_t,Task>> *newTasks);

        bool RobotsManagement::createRobot();
        bool RobotsManagement::destroyRobot();
        bool RobotsManagement::createTask(RobotFunctions funct);
        bool RobotsManagement::moveRobot(RobotFunctions funct,time_t id,int robotNo);
};
#endif