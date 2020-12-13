#ifndef ROBOTS_MANAGEMENT
#define ROBOTS_MANAGEMENT

#include "RobotFunctions.hpp"
#include "Task.hpp"
#include "VillageStats.hpp"
#include <vector>
#include <map>
#include <ctime>

namespace Application
{
    class RobotsManagement
    {

    private:
        static const int MAX_TASK_TYPES = 5;
        int totRobots;
        int freeRobots;
        int prodCost;
        VillageStats *villageStats;
        std::map<int, Task> tasks[MAX_TASK_TYPES];

    public:
        RobotsManagement();
        void initializeStats();
        ~RobotsManagement();

        int getTotRobots() const;
        int getFreeRobots() const;
        int getProdCost() const;
        std::map<int, Task> *getTasks() const;

        void setTotRobots(int newTotRobots);
        void setFreeRobots(int newFreeRobots);
        void setProdCost(int newProdCost);
        void setVillageStats(VillageStats *newVillageStats);
        void setTasks(std::vector<std::map<int, Task>> *newTasks);

        bool createRobots(int);
        bool destroyRobots(int);
        void createTask(RobotFunction);
        void updateTasks();
        bool moveRobot(Task &, int);
        bool endTask(Task &);
    };
} // namespace Application
#endif