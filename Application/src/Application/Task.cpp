#include "Application/header/Task.hpp"
#include "Application/header/ConstsMap.hpp"

#include "mypch.hpp"

namespace Application
{
    TaskID Task::s_NextID = 0;

    //Inicialização e destruição de classe
    Task::Task(RobotFunction funct, OnFinishedCallback onFinishedCallback) : 
        m_OnFinishedCallback(onFinishedCallback),
        TIME_STEP(ConstsMap::getValue("TIME_STEP")),
        INIT_TIME_STEP(ConstsMap::getValue("INIT_TIME_STEP")),
        MAX_TIME_STEPS(ConstsMap::getValue("MAX_TIME_STEPS")),
        MIN_REWARD(ConstsMap::getValue("MIN_REWARD")),
        REWARD_RANGE(ConstsMap::getValue("REWARD_RANGE")),
        FAILURE_TAX(ConstsMap::getValue("FAILURE_TAX"))
    {
        // use current time as seed for random generator
        std::srand(std::time(nullptr));
        initializeParameters(funct);

        m_Running = false;
    }

    Task::~Task() {
        static auto inertCallback = [](Task& _){};
        m_OnFinishedCallback = inertCallback;
        m_Running = false;
        DE_DEBUG("~Task()");
    }

    int Task::calcLostRobots()
    {
        int progress = (int)(FAILURE_TAX * 100 * ((float)curProgress / (float)progressLength));
        int randomLost = 1 + std::rand() % 100;
        float lostRobots = 0;

        if (progress > randomLost)
            lostRobots = (float)robotsNo * (((float)(progress - randomLost)) / 100.0);

        return (int)lostRobots <= robotsNo ? (int)lostRobots : robotsNo;
    }

    void Task::initializeParameters(RobotFunction funct)
    {
        id = s_NextID++;
        type = funct;
        robotsNo = 0;
        progressLength = TIME_STEP * (INIT_TIME_STEP + (std::rand() % MAX_TIME_STEPS));
        curProgress = 0;
        lastUpdateTime = time(0);
        remainingTime = -1;
        gainedGoods = 0;
        rewardMultiplier = MIN_REWARD + (std::rand() % REWARD_RANGE);
        predictedTime = 0;
    }

    //Gets of each parameter
    TaskID Task::getId() const
    {
        return id;
    }

    RobotFunction Task::getType() const
    {
        return type;
    }

    int Task::getRobotsNo() const
    {
        return robotsNo;
    }

    int Task::getAvgReward() const
    {
        return AVG_REWARD;
    }

    int Task::getProgressLength() const
    {
        return progressLength;
    }

    int Task::getCurProgress() const
    {
        return curProgress;
    }

    time_t Task::getLastUpdate() const
    {
        return lastUpdateTime;
    }

    time_t Task::getRemainingTime() const
    {
        return remainingTime;
    }

    float Task::getGainedGoods() const
    {
        return gainedGoods;
    }

    //Set of number of robots
    void Task::setRobotsNo(int newRobotsNo)
    {
        robotsNo = newRobotsNo;
    }

    bool Task::updateTask()
    {
        time_t curTime = time(0);
        // int oldProgress = curProgress;
        int progress = curProgress + (curTime - lastUpdateTime) * robotsNo;
        curProgress = progress > progressLength ? progressLength : progress;
        remainingTime = robotsNo==0 ? -1 : (progressLength - curProgress) / robotsNo;
        gainedGoods = (pow((float)curProgress, 2) / (float)progressLength) * (float)rewardMultiplier;
        lastUpdateTime = curTime;
        return remainingTime != 0;
    }


    

    //TODO: Implmentar a criação e destruição da thread e como ela funciona

    void Task::threadLoop() {
        
        while (true) {
            if (!m_Running || !updateTask()) break;
            sleep(1);
        }

        DE_DEBUG("(Task) fim da thread {0}", id);
        try{
            if (m_OnFinishedCallback != nullptr)
                m_OnFinishedCallback(*this);
        } catch (const std::exception &e) {
            DE_ERROR("Task::m_OnFinishedCallback emmitted an exception\n: {0}", e.what());
        }
    }

    void Task::start() {
        DE_TRACE("Starting Task #{0} (Function:{1})", id, getRobotFunctionString(type) );
        m_Running = true;
        pthread_create(&m_TaskThread, NULL, runThreadLoop, this);
    }

    void Task::stop() {
        DE_TRACE("Stopping Task #{0} (Function:{1})", id, getRobotFunctionString(type) );
        m_Running = false;
    }

    void Task::detach() {
        DE_TRACE("Detaching Task #{0} (Function:{1})", id, getRobotFunctionString(type));
        m_OnFinishedCallback = nullptr;
    }

    void *runThreadLoop(void *taskObject) {
        Task *task = (Task*)taskObject;
        task->threadLoop();
        return NULL;
    }

} // namespace Application