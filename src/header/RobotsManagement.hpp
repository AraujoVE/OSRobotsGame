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
        bool inProgress;
        int totRobots;
        int freeRobots;
        int efficiency;
        int prodCost;
    public:
        bool RobotsManagement::createRobot(VillageStats vila);
        bool RobotsManagement::destroyRobot();
        bool RobotsManagement::sendRobot(std::vector<std::map<tm,Task>> tasks,RobotFunctions funct,int robotNo);
}