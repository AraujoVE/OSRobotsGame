#include "Application/Game/Ingame/RobotsManagement.hpp"

#include "Application/Game/GameConsts.hpp"

#include "mypch.hpp"
//Inicialização e destruição da classe
namespace Application
{

    RobotsManagement::RobotsManagement(GameConsts& gameConsts)
        : m_GameConstsCache(gameConsts)
    {
        initializeStats();
    }

    void RobotsManagement::initializeStats()
    {
        totRobots =  m_GameConstsCache.TOT_ROBOTS_INI;
        freeRobots =  m_GameConstsCache.TOT_ROBOTS_INI;
        prodCost = m_GameConstsCache. PROD_COST_INI;
        m_VillageStatsMutex.lock();
        villageStats = NULL;
        m_VillageStatsMutex.unlock();

    }

    RobotsManagement::~RobotsManagement() {
        for (int i = 0; i < FUNCTION_QTY; i++) {
            auto &taskMap = tasks[i];
            for (auto taskP : taskMap) {
                delete taskP.second;
            }
            taskMap.clear();
        }

        //Assures no one is going to receive zombie objects events
        m_EventListener.Clear();
    }

    //Gets de cada um dos parâmetros

    uint64_t RobotsManagement::getTotRobots() const
    {
        return totRobots;
    }

    uint64_t RobotsManagement::getFreeRobots() const
    {
        return freeRobots;
    }


    bool RobotsManagement::canRemoveRobots() const{
        robotsMutexes[FREE_ROBOTS].lock();
        bool canRemove = freeRobots > 0;
        robotsMutexes[FREE_ROBOTS].unlock();
        return canRemove;
    }

    bool RobotsManagement::canAddRobots() {
        bool canAdd = false;
        m_VillageStatsMutex.lock();
        robotsMutexes[PROD_COST].lock();
        {
            if (villageStats != nullptr) {
                villageStats->getMutex(RobotFunction::RESOURCE_GATHERING).lock();
                canAdd = villageStats->getResources() >= prodCost;
                villageStats->getMutex(RobotFunction::RESOURCE_GATHERING).unlock();
            } else {
                DE_WARN("(RobotsManagement::canAddRobots) return false because villageStats is nullptr");
            }
        }
        robotsMutexes[PROD_COST].unlock();
        m_VillageStatsMutex.unlock();

        return canAdd;
    }

    void RobotsManagement::clearEvents() { m_EventListener.Clear(); }

    void RobotsManagement::setOnTaskCreated(EH_TaskCreated* eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnTaskEnded(EH_TaskFinished* eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnTaskCancelled(EH_TaskCancelled* eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsCreated(EH_RobotsCreated* eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsDestroyed(EH_RobotsDestroyed* eHandler) { m_EventListener.Register(eHandler); }
    void RobotsManagement::setOnRobotsMoved(EH_RobotsMoved* eHandler) { m_EventListener.Register(eHandler); }


    uint64_t RobotsManagement::getProdCost() const
    {
        return prodCost;
    }

    inline const std::unordered_map<Task::TaskID, Task*> &RobotsManagement::getTasks(RobotFunction function) const {
        return tasks[(int)function];
    }

    std::optional<Task*> RobotsManagement::findTask(Task::TaskID taskID, RobotFunction functionHint) const {
        DE_ASSERT(FUNCTION_QTY > 0);

        tasksDown();

        for (int i = (int)functionHint; i < FUNCTION_QTY + (int) functionHint; i++)
        {
            auto searchRes = tasks[i%FUNCTION_QTY].find(taskID);
            if (searchRes != tasks[i%FUNCTION_QTY].end()) {
                tasksUp();
                return std::optional<Task*>{(Task*)searchRes->second};
            }
        }

        tasksUp();

        return std::nullopt;
    }
    

    //Sets de cada um dos parâmetros

    void RobotsManagement::setTotRobots(uint64_t newTotRobots)
    {
        totRobots = newTotRobots;
    }

    void RobotsManagement::setFreeRobots(uint64_t newFreeRobots)
    {
        freeRobots = newFreeRobots;
    }

    void RobotsManagement::setProdCost(uint64_t newProdCost)
    {
        prodCost = newProdCost;
    }

    void RobotsManagement::setVillageStats(VillageStats *newVillageStats)
    {
        //TODO: call this function when VS is destroyed (setVillageStats(nullptr))
        m_VillageStatsMutex.lock();
        {
            villageStats = newVillageStats;
        }
        m_VillageStatsMutex.unlock();
    }

    bool RobotsManagement::createRobots(uint64_t no)
    {   
        int currentResources;
        int price;
        std::mutex *VSResourcesMutex = nullptr;
        bool hasCreated = false;

        price = prodCost * no;
        m_VillageStatsMutex.lock();
        {
            if (villageStats == nullptr) {
                DE_WARN("(RobotsManagement) createRobots failed because villageStats == nullptr");
                m_VillageStatsMutex.unlock();
                return false;
            }

            VSResourcesMutex = &villageStats->getMutex(RobotFunction::RESOURCE_GATHERING);
            VSResourcesMutex->lock();
            currentResources = villageStats->getResources();

            //Calcula se existe dinheiro o suficiente para criar um robô
            if (currentResources >= price)
            {
                //Decrementa dos recursos o custo de produção do robô
                villageStats->setResources(currentResources-price);
                VSResourcesMutex->unlock();
                
                //Incrementa o número total e o número de robôs livres
                changeRobotsNum(TOT_ROBOTS, no);
                changeRobotsNum(FREE_ROBOTS, no);

                hasCreated = true;
            } else {
                VSResourcesMutex->unlock();
            }

        }
        m_VillageStatsMutex.unlock();


        m_EventListener.On<EH_RobotsCreated>(no);

        return hasCreated;
    }

    bool RobotsManagement::destroyRobots(uint64_t no)
    {
        bool hasDestroyed = false;

        robotsMutexes[TOT_ROBOTS].lock();
        robotsMutexes[FREE_ROBOTS].lock();

        //Verifica se é possível destruir robôs
        if (totRobots >= no && freeRobots >= no)
        {
            //Decrementa o número total e o número de robôs livres
            totRobots -= no;
            freeRobots -= no;
            hasDestroyed = true;
        } else DE_WARN("Trying to destroy {0} from {1} robots, ignoring...", no, totRobots);
        
        robotsMutexes[TOT_ROBOTS].unlock();
        robotsMutexes[FREE_ROBOTS].unlock();

        if (hasDestroyed)
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
        Task *newTask = new Task(funct, m_GameConstsCache, m_NextTaskID++);

        newTask->m_EventListener->Register(new EH_TaskFinished(std::bind(&RobotsManagement::onTaskEnded<EH_TaskFinished>, this, std::placeholders::_1)));
        newTask->m_EventListener->Register(new EH_TaskCancelled(std::bind(&RobotsManagement::onTaskEnded<EH_TaskCancelled>, this, std::placeholders::_1)));

        tasksDown();
        tasks[(int)funct][newTask->GetID()] = newTask;
        tasksUp();

        auto *el = &m_EventListener;
        newTask->m_EventListener->Register(new EH_TaskStarted([=](Task& task){
            el->On<EH_TaskStarted>(task);
            return false;
        }));

        m_EventListener.On<EH_TaskCreated>(*newTask);
        newTask->Start();

        return true;
    }

    void RobotsManagement::cancelTask(Task &task) {
        task.Cancel();
    }

    void RobotsManagement::taskEndedRoutine(Task &endedTask)
    {
        int lostRobots = endedTask.CalcLostRobots();
        moveRobot(endedTask, -1 * endedTask.GetRobotsNo());
        destroyRobots(lostRobots);

        if(endedTask.GetRobotFunction() == RobotFunction::RESOURCE_GATHERING){
            robotsMutexes[PROD_COST].lock();
            float increase = 1.0 + m_GameConstsCache.PROD_COST_INCREASE_TAX*(endedTask.GetGainedGoods()/(float)endedTask.GetAvgReward()) ;
            prodCost = (int)((float)prodCost * increase);
            robotsMutexes[PROD_COST].unlock();
        }

        m_VillageStatsMutex.lock();
        {
            if (villageStats != nullptr) villageStats->applyGainedGoods(endedTask.GetRobotFunction(), (int)endedTask.GetGainedGoods());
            else { DE_WARN("(RobotsManagement::taskEndedRoutine) ignoring villageStats->applyGainedGoods, since villageStats is nullptr"); }
        }
        m_VillageStatsMutex.unlock();


        auto &map = tasks[(int)endedTask.GetRobotFunction()];
        Task::TaskID id = endedTask.GetID();
        //TODO: fix free 
        // delete &endedTask;

        map.erase(id); 
    }
    
    bool RobotsManagement::moveRobot(Task &choosenTask, int64_t robotsNo)
    {   
        if (robotsNo == 0)
            return true; //If no robots are added or removed, nothing to do
        
        bool returnValue = true;

        robotsMutexes[FREE_ROBOTS].lock();

        if (robotsNo > 0 && freeRobots < (uint64_t) robotsNo)
            returnValue = false; // Can't add robots to a task if there are not enough free robots
        else if (robotsNo < 0 && choosenTask.GetRobotsNo() + robotsNo < 0)
            returnValue = false; //Can't remove robots from a task if there are not enough robots in the given task
        else if (tasks[(int)choosenTask.GetRobotFunction()].find(choosenTask.GetID()) == tasks[(int)choosenTask.GetRobotFunction()].end())
            returnValue = false; //Can't move or remove robots from an inexistent task.
        else
            freeRobots -= robotsNo;

        //Add or remove a robot from a given task
        if (returnValue == true)
            choosenTask.setRobotsNo(choosenTask.GetRobotsNo() + robotsNo);
            
        robotsMutexes[FREE_ROBOTS].unlock(); 

        if (returnValue == true) {
            DE_TRACE("Moving {0} robots to task #{1}", robotsNo, choosenTask.GetID());
            m_EventListener.On<EH_RobotsMoved>({choosenTask, robotsNo});
        } else {
            DE_WARN("Ignoring invalid attempt to move {0} robots to task #{1}", robotsNo, choosenTask.GetID());
        }


        return returnValue;
    }

    void RobotsManagement::changeRobotsNum (int type, int increase) {
        std::lock_guard<std::mutex> guard(robotsMutexes[type]);
        (*robotVars[type]) += increase;
    }

    void RobotsManagement::tasksUp() const {
        tasksMutex.unlock();
    } 

    void RobotsManagement::tasksDown() const {
        tasksMutex.lock();
    }

} // namespace Application