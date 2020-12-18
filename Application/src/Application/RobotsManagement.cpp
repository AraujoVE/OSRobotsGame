#include "mypch.hpp"

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
        //TODO: mudar de volta (para 1, tot e free)
        totRobots = 10;
        freeRobots = 10;
        prodCost = 1;
        villageStats = NULL;
        m_FunctionWindowArray = nullptr;
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

    void RobotsManagement::setFunctionWindowsArray(FunctionWindow **functionWindowArray) {
        m_FunctionWindowArray = functionWindowArray;
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

    Task& RobotsManagement::createTask(RobotFunction funct)
    {   
        DE_TRACE("(RobotsManagement) createTask() ");

        //Cria nova task com o id Incrementado
        Task *newTask = new Task(funct, std::bind(&RobotsManagement::onTaskEnded, this, std::placeholders::_1));
        

        tasksDown();
        tasks[(int)funct][newTask->getId()] = newTask;
        tasksUp();

        newTask->start();

        return *newTask;
    }

    void RobotsManagement::endTask(Task &task) {
        DE_DEBUG("(RobotsManagement) Solicitada endtask()");
        task.stop();
    }

    void RobotsManagement::onTaskEnded(Task &endedTask)
    {
        DE_DEBUG("(RobotsManagement) onTaskEnded()");

        int lostRobots = endedTask.calcLostRobots();
        moveRobot(endedTask, -1 * endedTask.getRobotsNo());
        destroyRobots(lostRobots);
        villageStats->changeStat((int)endedTask.getType(), (int)endedTask.getGainedGoods());

        DE_ASSERT(m_FunctionWindowArray != nullptr, "FunctionWindowArray is not set");

        m_FunctionWindowArray[(int)endedTask.getType()]->OnTaskEnded(endedTask);
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