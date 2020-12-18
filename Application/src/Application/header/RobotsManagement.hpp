#ifndef ROBOTS_MANAGEMENT
#define ROBOTS_MANAGEMENT

#include "RobotFunctions.hpp"
#include "Task.hpp"
#include "VillageStats.hpp"
#include "Avenue.hpp"
#include <vector>
#include "Application/ImGui/Window/FunctionWindow.hpp"

#include <unordered_map>

namespace Application
{
    class RobotsManagement
    {
    public:
        enum Robots_Values_Index {TOT_ROBOTS, FREE_ROBOTS};

        static const int FUNCTION_QTY = 5;
        static const int MAX_TASKS_PER_FUNCTION = 5;

    private:
        int totRobots;
        int freeRobots;
        int prodCost; //TODO: Implementation to change this value
        VillageStats *villageStats;
        std::unordered_map<TaskID, Task*> tasks[FUNCTION_QTY];
        Avenue<int> *robotsAvenues[2];
        pthread_t consumers[2];
        pthread_mutex_t tasksMutex;

        void initializeAvenues();
        void changeRobotsNum (int type, int increase);
        FunctionWindow **m_FunctionWindowArray;

    public:
        
        RobotsManagement();
        void initializeStats();
        ~RobotsManagement();

        int getTotRobots() const;
        int getFreeRobots() const;
        int getProdCost() const;
        const std::unordered_map<TaskID, Task*> &getTasks(RobotFunction function) const;
        Task &findTask(TaskID taskID) const;

        void onTaskCompleted(TaskID completedTaskID);
        void setTotRobots(int newTotRobots);
        void setFreeRobots(int newFreeRobots);
        void setProdCost(int newProdCost);
        void setVillageStats(VillageStats *newVillageStats);
        void setFunctionWindowsArray(FunctionWindow **FunctionWindowArray);
        //TODO: determine if it will be in fact allowed to set tasks
        // void setTasks(std::vector<std::unordered_map<int, Task>> *newTasks);

        bool createRobots(int);
        bool destroyRobots(int);
        Task& createTask(RobotFunction);
        bool moveRobot(Task &, int);

        void tasksUp();
        void tasksDown();
        
        void endTask(Task &);
        void onTaskEnded(Task &);
    };
} // namespace Application
#endif