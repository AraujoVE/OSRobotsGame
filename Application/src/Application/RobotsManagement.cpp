#include "header/RobotsManagement.hpp"
#include "header/Task.hpp"
#include "header/VillageStats.hpp"
#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include <iterator>

//Inicialização e destruição da classe
namespace Application
{

    RobotsManagement::RobotsManagement()
    {
        initializeStats();
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
        int curResources = villageStats->getStat((int)RobotFunction::RESOURCE_GATHERING);

        //Calcula se existe dinheiro o suficiente para criar um robô
        if (curResources >= prodCost * no)
        {
            //Decrementa dos recursos o custo de produção do robô
            //Mudar para changeStats
            villageStats->changeStat((int)RobotFunction::RESOURCE_GATHERING,-1*prodCost*no);
            //Incrementa o número total e o número de robôs livres
            totRobots += no;
            freeRobots += no;
            return true;
        }
        return false;
    }

    bool RobotsManagement::destroyRobots(int no)
    {
        //Verifica se é possível destruir robôs
        if (totRobots >= no && freeRobots >= no)
        {
            //Decrementa o número total e o número de robôs livres
            totRobots -= no;
            freeRobots -= no;
            return true;
        }
        return false;
    }

    void RobotsManagement::createTask(RobotFunction funct)
    {
        //Cria nova task com o id Incrementado
        Task newTask(funct);
        tasks[(int)funct].insert({newTask.getId(), newTask});
    }

    bool RobotsManagement::endTask(Task &curTask)
    {
        if (!curTask.updateTask())
        {
            int lostRobots = curTask.calcLostRobots();
            moveRobot(curTask, -1 * curTask.getRobotsNo());
            destroyRobots(lostRobots);
            villageStats->changeStat((int)curTask.getType(), (int)curTask.getGainedGoods());

            return true;
        }
        return false;
    }

    //This method is probably useless once each task will have a thread
    void RobotsManagement::updateTasks()
    {
        bool finishedTask = false;
        for (std::map<int, Task> &funct : tasks)
        {
            //E se itera em cada robo de dada função
            std::map<int, Task>::iterator itr = funct.begin();
            while (itr != funct.end())
            {
                if (endTask(itr->second))
                    itr = funct.erase(itr);
                else
                    ++itr;
            }
        }
    }

    bool RobotsManagement::moveRobot(Task &choosenTask, int robotsNo)
    {
        if (!robotsNo)
            return true; //If no robots are add or removed, nothing to do
        if (robotsNo > 0 && freeRobots < robotsNo)
            return false; // Can't add robots to a task if there are not enough free robots
        else if (robotsNo < 0 && choosenTask.getRobotsNo() + robotsNo < 0)
            return false; //Can't remove robots from a task if there are not enough robots in the given task
        else if (tasks[(int)choosenTask.getType()].find(choosenTask.getId()) == tasks[(int)choosenTask.getType()].end())
            return false; //Can't move or remove robots from an inexistent task.

        //Incrementa or decrement the number of free robots
        this->setFreeRobots(freeRobots - robotsNo);

        //Add or remove a robot from a given task
        choosenTask.setRobotsNo(choosenTask.getRobotsNo() + robotsNo);

        return true;
    }

} // namespace Application