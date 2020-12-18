#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stdexcept>

//Inicialização e destruição da classe
namespace Application
{

    RobotsManagement::RobotsManagement()
    {
        initializeStats();
        initializeAvenues();

        pthread_mutex_init(&tasksMutex, NULL);
    }

    void RobotsManagement::initializeAvenues() {
        robotsAvenues[TOT_ROBOTS] = new Avenue(totRobots);
        robotsAvenues[FREE_ROBOTS] = new Avenue(freeRobots);

        pthread_create(&consumers[TOT_ROBOTS], NULL, runConsumer, robotsAvenues[TOT_ROBOTS]);
        pthread_create(&consumers[FREE_ROBOTS], NULL, runConsumer, robotsAvenues[FREE_ROBOTS]);
    }

    void RobotsManagement::initializeStats()
    {
        totRobots = 1;
        freeRobots = 1;
        prodCost = 1;
        villageStats = NULL;
    }

    RobotsManagement::~RobotsManagement() {}

    //Gets de cada um dos parâmetros

    int RobotsManagement::getTotRobots() const
    {
        return totRobots;
    }

    int RobotsManagement::getFreeRobots() const
    {
        return freeRobots;
    }

    int RobotsManagement::getProdCost() const
    {
        return prodCost;
    }

    inline const std::unordered_map<TaskID, Task*> &RobotsManagement::getTasks(RobotFunction function) const {
        return tasks[(int)function];
    }

    Task &RobotsManagement::findTask(TaskID taskID) const {
        for (int i = 0; i < FUNCTION_QTY; i++)
        {
            auto searchRes = tasks[i].find(taskID);
            if (searchRes != tasks[i].end()) {
                return *(Task*)searchRes->second;
            }
        }

        throw std::logic_error("Task not found");
    }
    

    //Sets de cada um dos parâmetros

    void RobotsManagement::setTotRobots(int newTotRobots)
    {
        totRobots = newTotRobots;
    }

    void RobotsManagement::setFreeRobots(int newFreeRobots)
    {
        freeRobots = newFreeRobots;
    }

    void RobotsManagement::setProdCost(int newProdCost)
    {
        prodCost = newProdCost;
    }

    void RobotsManagement::setVillageStats(VillageStats *newVillageStats)
    {
        villageStats = newVillageStats;
    }

    bool RobotsManagement::createRobots(int no)
    {   
        int currentResources;
        int price;
        Avenue *VSResourcesAvenue;
        bool hasCreated = false;


        price = prodCost * no;
        VSResourcesAvenue = villageStats->getAvenue((int)RobotFunction::RESOURCE_GATHERING);
        VSResourcesAvenue->down();
        currentResources = villageStats->getResources();

        //Calcula se existe dinheiro o suficiente para criar um robô
        if (currentResources >= price)
        {
            //Decrementa dos recursos o custo de produção do robô
            villageStats->setResources(currentResources-price);
            VSResourcesAvenue->up();
            
            //Incrementa o número total e o número de robôs livres
            changeRobotsNum(TOT_ROBOTS, no);
            changeRobotsNum(FREE_ROBOTS, no);

            hasCreated = true;
        } else {
            VSResourcesAvenue->up();
        }


        return hasCreated;
    }

    bool RobotsManagement::destroyRobots(int no)
    {
        bool hasDestroyed = false;

        robotsAvenues[TOT_ROBOTS]->down();
        robotsAvenues[FREE_ROBOTS]->down();

        //Verifica se é possível destruir robôs
        if (totRobots >= no && freeRobots >= no)
        {
            //Decrementa o número total e o número de robôs livres
            totRobots -= no;
            freeRobots -= no;
            hasDestroyed = true;
        }
        
        robotsAvenues[TOT_ROBOTS]->up();
        robotsAvenues[FREE_ROBOTS]->up();

        return hasDestroyed;
    }

    void RobotsManagement::onTaskCompleted(TaskID completedTaskID) {
        //TODO: AraujoVE logic
    }

    void RobotsManagement::createTask(RobotFunction funct)
    {
        //Cria nova task com o id Incrementado
        Task *newTask = new Task(funct, std::bind(&RobotsManagement::onTaskCompleted, this, std::placeholders::_1));

        tasksDown();
        tasks[(int)funct][newTask->getId()] = newTask;
        tasksUp();
    }

    bool RobotsManagement::endTask(Task &curTask)
    {
        bool hasEnded = false;
        int lostRobots;
        
        if (!curTask.updateTask())
        {   
            lostRobots = curTask.calcLostRobots();
            moveRobot(curTask, -1 * curTask.getRobotsNo());
            destroyRobots(lostRobots);
            villageStats->changeStat((int)curTask.getType(), (int)curTask.getGainedGoods());

            hasEnded = true;
        }

        return hasEnded;
    }

    //This method is probably useless once each task will have a thread
    void RobotsManagement::updateTasks()
    {
        for (int functIdx = 0; functIdx < FUNCTION_QTY; functIdx++)
        {
            //E se itera em cada robo de dada função
            std::unordered_map<TaskID, Task*>::iterator itr = tasks[functIdx].begin();
            while (itr != tasks[functIdx].end())
            {
                if (endTask(*itr->second))
                    itr = tasks[functIdx].erase(itr);
                else
                    ++itr;
            }
        }
    }

    bool RobotsManagement::moveRobot(Task &choosenTask, int robotsNo)
    {   
        if (!robotsNo)
            return true; //If no robots are add or removed, nothing to do
        
        bool returnValue = true;

        robotsAvenues[FREE_ROBOTS]->down();

        if (robotsNo > 0 && freeRobots < robotsNo)
            returnValue = false; // Can't add robots to a task if there are not enough free robots
        else if (robotsNo < 0 && choosenTask.getRobotsNo() + robotsNo < 0)
            returnValue = false; //Can't remove robots from a task if there are not enough robots in the given task
        else if (tasks[(int)choosenTask.getType()].find(choosenTask.getId()) == tasks[(int)choosenTask.getType()].end())
            returnValue = false; //Can't move or remove robots from an inexistent task.
        else
            freeRobots -= robotsNo;

        robotsAvenues[FREE_ROBOTS]->up();

        //Add or remove a robot from a given task
        choosenTask.setRobotsNo(choosenTask.getRobotsNo() + robotsNo);

        return returnValue;
    }

    void RobotsManagement::changeRobotsNum (int type, int increase) {
        robotsAvenues[type]->producer(increase);
    }

    void RobotsManagement::tasksUp() {
        pthread_mutex_unlock(&tasksMutex);
    } 

    void RobotsManagement::tasksDown() {
        pthread_mutex_lock(&tasksMutex);
    }

} // namespace Application