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
    public:
        static const int FUNCTION_QTY = 5;
        static const int MAX_TASKS_PER_FUNCTION = 5;

    private:
        int totRobots;
        int freeRobots;
        int prodCost; //TODO: Implementation to change this value
        VillageStats *villageStats;
        std::map<TaskID, Task> tasks[FUNCTION_QTY];

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