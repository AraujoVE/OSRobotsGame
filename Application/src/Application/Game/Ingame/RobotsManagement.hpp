#pragma once

#include "RobotFunctions.hpp"
#include "Task.hpp"
#include "VillageStats.hpp"
#include "Application/Threads/Avenue.hpp"
#include <vector>
#include "Application/ImGui/GameWindows/FunctionWindow.hpp"
#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include <optional>

#include <unordered_map>

namespace Application
{

    class RobotsManagement
    {
    public:
        enum Robots_Values_Index {TOT_ROBOTS, FREE_ROBOTS, PROD_COST};

        static constexpr int MAX_TASKS_PER_FUNCTION = 5;

    private:
        int m_NextTaskID = 0;

        int totRobots;
        int freeRobots;
        int prodCost; //TODO: Implementation to change this value

        GameConstsCache m_GameConstsCache;

        VillageStats *villageStats;
        std::unordered_map<Task::TaskID, Task*> tasks[FUNCTION_QTY];
        Avenue *robotsAvenues[3];
        pthread_t consumers[3];
        pthread_mutex_t tasksMutex;

        void initializeAvenues();
        void changeRobotsNum (int type, int increase);
        EventListener m_EventListener;

    public:
        
        RobotsManagement(GameConsts& gameConsts);
        void initializeStats();
        ~RobotsManagement();

        int getTotRobots() const;
        int getFreeRobots() const;
        int getProdCost() const;
        
        bool canRemoveRobots() const;
        bool canAddRobots() const;

        void setOnTaskCreated(EH_TaskCreated*);
        void setOnTaskEnded(EH_TaskFinished*);
        void setOnTaskCancelled(EH_TaskCancelled*);
        void setOnRobotsCreated(EH_RobotsCreated*);
        void setOnRobotsDestroyed(EH_RobotsDestroyed*);
        void setOnRobotsMoved(EH_RobotsMoved*);

        

        void setTotRobots(int newTotRobots);
        void setFreeRobots(int newFreeRobots);
        void setProdCost(int newProdCost);
        void setVillageStats(VillageStats *newVillageStats);
        
        bool createRobots(int);
        bool destroyRobots(int);
        bool moveRobot(Task &, int);

        const std::unordered_map<Task::TaskID, Task*> &getTasks(RobotFunction function) const;
        std::optional<Task*> findTask(Task::TaskID taskID, RobotFunction functionHint = (RobotFunction) 0) const;

        
        bool createTask(RobotFunction);
        void cancelTask(Task &);

    private:
        void tasksUp() const;
        void tasksDown() const;

        void taskEndedRoutine(Task &);
        
        template<typename Event> //Event == EH_TaskFinished | EH_TaskCancelled
        inline bool onTaskEnded(Task& task) { m_EventListener.On<Event>(task); taskEndedRoutine(task); return false; }
    };
} // namespace Application