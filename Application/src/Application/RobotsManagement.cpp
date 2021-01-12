#include "Application/header/RobotsManagement.hpp"
#include "mypch.hpp"

//Inicialização e destruição da classe
namespace Application
{

    RobotsManagement::RobotsManagement():
        PROD_COST_INCREASE_TAX(ConstsMap::getValue("PROD_COST_INCREASE_TAX")),
        TOT_ROBOTS_INI(ConstsMap::getValue("TOT_ROBOTS_INI")),
        FREE_ROBOTS_INI(ConstsMap::getValue("FREE_ROBOTS_INI")),
        PROD_COST_INI(ConstsMap::getValue("PROD_COST_INI")) 
    {
        initializeStats();
        initializeAvenues();

        pthread_mutex_init(&tasksMutex, NULL);
    }

    void RobotsManagement::initializeAvenues()
    {
        robotsAvenues[TOT_ROBOTS] = new Avenue(totRobots);
        robotsAvenues[FREE_ROBOTS] = new Avenue(freeRobots);
        robotsAvenues[PROD_COST] = new Avenue(prodCost);


        robotsAvenues[TOT_ROBOTS]->startConsumer();
        robotsAvenues[FREE_ROBOTS]->startConsumer();
        robotsAvenues[PROD_COST]->startConsumer();
    }

    void RobotsManagement::initializeStats()
    {
        
        totRobots = TOT_ROBOTS_INI;
        freeRobots = FREE_ROBOTS_INI;
        prodCost = PROD_COST_INI;
        villageStats = NULL;
    }

    RobotsManagement::~RobotsManagement() {
        robotsAvenues[TOT_ROBOTS]->stopConsumer();
        robotsAvenues[FREE_ROBOTS]->stopConsumer();
        robotsAvenues[PROD_COST]->stopConsumer();

        delete robotsAvenues[TOT_ROBOTS];
        delete robotsAvenues[FREE_ROBOTS];
        delete robotsAvenues[PROD_COST];


        for (int i = 0; i < FUNCTION_QTY; i++) {
            auto &taskMap = tasks[i];
            for (auto taskP : taskMap) {
                delete taskP.second;
            }
            taskMap.clear();
        }
    }

    //Gets de cada um dos parâmetros

    int RobotsManagement::getTotRobots() const
    {
        return totRobots;
    }

    int RobotsManagement::getFreeRobots() const
    {
        return freeRobots;
    }


    bool RobotsManagement::canRemoveRobots() const{
        robotsAvenues[FREE_ROBOTS]->down();
        bool canRemove = freeRobots > 0;
        robotsAvenues[FREE_ROBOTS]->up();
        return canRemove;
    }

    bool RobotsManagement::canAddRobots() const{
        robotsAvenues[PROD_COST]->down();
        villageStats->getAvenue((int)RobotFunction::RESOURCE_GATHERING)->down();
        bool canAdd = villageStats->getResources() >= prodCost;
        villageStats->getAvenue((int)RobotFunction::RESOURCE_GATHERING)->up();
        robotsAvenues[PROD_COST]->up();

        return canAdd;
    }

    void RobotsManagement::setOnTaskCreated(EventHandler<bool(Task&)> *eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnTaskEnded(EventHandler<bool(Task&)> *eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsCreated(EventHandler<bool(int count)> *eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsDestroyed(EventHandler<bool(int count)> *eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsMoved(EventHandler<bool(Task& targetTask, int count)> *eHandler) { m_EventListener.Register(eHandler); }


    int RobotsManagement::getProdCost() const
    {
        return prodCost;
    }

    inline const std::unordered_map<TaskID, Task*> &RobotsManagement::getTasks(RobotFunction function) const {
        return tasks[(int)function];
    }

    Task &RobotsManagement::findTask(TaskID taskID) const {
        tasksDown();

        for (int i = 0; i < FUNCTION_QTY; i++)
        {
            auto searchRes = tasks[i].find(taskID);
            if (searchRes != tasks[i].end()) {
                tasksUp();
                return *(Task*)searchRes->second;
            }
        }

        tasksUp();

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


        m_EventListener.On<EH_RobotsCreated>(no);

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

        m_EventListener.On<EH_RobotsDestroyed>(no);

        return hasDestroyed;
    }

    bool RobotsManagement::createTask(RobotFunction funct)
    {   
        //Ignora pedidos que lotariam a tela
        if (tasks[(int)funct].size() >= RobotsManagement::MAX_TASKS_PER_FUNCTION) {
            DE_WARN("(RobotsManagement) Ignoring task creation request (limit of {0} exceeded)", RobotsManagement::MAX_TASKS_PER_FUNCTION);
            return false;
        }

        //Cria nova task com o id Incrementado
        Task *newTask = new Task(funct, std::bind(&RobotsManagement::onTaskEnded, this, std::placeholders::_1));

        tasksDown();
        tasks[(int)funct][newTask->getId()] = newTask;
        tasksUp();

        newTask->start();

        m_EventListener.On<EH_TaskCreated, Task&>(*newTask);

        return true;
    }

    void RobotsManagement::endTask(Task &task) {
        task.stop();
    }

    void RobotsManagement::onTaskEnded(Task &endedTask)
    {
        int lostRobots = endedTask.calcLostRobots();
        moveRobot(endedTask, -1 * endedTask.getRobotsNo());
        destroyRobots(lostRobots);

        if(endedTask.getType() == RobotFunction::RESOURCE_GATHERING){
            robotsAvenues[PROD_COST]->down();
            float increase = 1.0 + PROD_COST_INCREASE_TAX*(endedTask.getGainedGoods()/(float)endedTask.getAvgReward()) ;
            prodCost = (int)((float)prodCost * increase);
            robotsAvenues[PROD_COST]->up();
        }

        villageStats->changeStat((int)endedTask.getType(), (int)endedTask.getGainedGoods());

        m_EventListener.On<EH_TaskEnded, Task&>(endedTask);
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

        //Add or remove a robot from a given task
        if (returnValue == true)
            choosenTask.setRobotsNo(choosenTask.getRobotsNo() + robotsNo);
            
        robotsAvenues[FREE_ROBOTS]->up(); 

        if (returnValue == true)
            m_EventListener.On<EH_RobotsMoved, Task&, int>(choosenTask, robotsNo);


        return returnValue;
    }

    void RobotsManagement::changeRobotsNum (int type, int increase) {
        robotsAvenues[type]->producer(increase);
    }

    void RobotsManagement::tasksUp() const {
        pthread_mutex_unlock((pthread_mutex_t*)&tasksMutex);
    } 

    void RobotsManagement::tasksDown() const {
        pthread_mutex_lock((pthread_mutex_t*)&tasksMutex);
    }

} // namespace Application