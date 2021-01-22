#include "Application/Game/Ingame/Task.hpp"
#include "Application/Game/GameConsts.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "DampEngine/Core/Macros/Log.hpp"

#include "mypch.hpp"

namespace Application
{
    //Inicialização e destruição de classe
    Task::Task(RobotFunction funct, GameConstsCache &gameConsts, Task::TaskID id)
        : m_GameConstsCache(gameConsts),
          id(id),
          function(funct)
    {
        // use current time as seed for random generator
        std::srand(std::time(nullptr));

        robotsNo = 0;
        progressLength = m_GameConstsCache.TIME_STEP * (m_GameConstsCache.INIT_TIME_STEP + (std::rand() % m_GameConstsCache.MAX_TIME_STEPS));
        curProgress = 0;
        lastUpdateTime = time(0);
        remainingTime = -1;
        gainedGoods = 0;
        rewardMultiplier = m_GameConstsCache.MIN_REWARD + (std::rand() % m_GameConstsCache.REWARD_RANGE);
        predictedTime = 0;

        m_EventListener = new EventListener();

        //TODO: fix memory leak
        //TODO: release those functions from threadloop when this object is dead

        m_ThreadLoop = new ThreadLoop(std::string("Task #") + std::to_string(id));

        m_ThreadLoop->SetTickFunction(std::bind(&Task::UpdateTask, this));
        m_ThreadLoop->SetAliveCheckFunction([this] { return !IsTaskCompleted(); });

        m_ThreadLoop->m_EventListener->Register(new EH_ThreadStarted(std::bind(&Task::OnThreadLoopStarted, this)));
        m_ThreadLoop->m_EventListener->Register(new EH_ThreadEnded(std::bind(&Task::OnThreadLoopEnded, this, std::placeholders::_1)));
    }

    Task::~Task()
    {
        m_ELMutex.Lock();
        {
            delete m_EventListener;
            m_EventListener = nullptr;
        }
        m_ELMutex.Unlock();

        m_ThreadLoop->Abandon();
    }

    int Task::CalcLostRobots()
    {
        int progress = (int)(m_GameConstsCache.FAILURE_TAX * 100 * ((float)curProgress / (float)progressLength));
        int randomLost = 1 + std::rand() % 100;
        float lostRobots = 0;

        if (progress > randomLost)
            lostRobots = (float)robotsNo * (((float)(progress - randomLost)) / 100.0);

        return (int)lostRobots <= robotsNo ? (int)lostRobots : robotsNo;
    }

    //Set of number of robots
    void Task::setRobotsNo(int newRobotsNo)
    {
        robotsNo = newRobotsNo;
    }

    void Task::UpdateTask()
    {
        time_t curTime = time(0);
        // int oldProgress = curProgress;
        int progress = curProgress + (curTime - lastUpdateTime) * robotsNo;
        curProgress = progress > progressLength ? progressLength : progress;
        remainingTime = robotsNo == 0 ? -1 : (progressLength - curProgress) / robotsNo;
        gainedGoods = (pow((float)curProgress, 2) / (float)progressLength) * (float)rewardMultiplier;
        lastUpdateTime = curTime;
    }

    void Task::Start()
    {
        DE_TRACE("Starting Task #{0} (Function:{1})", id, getRobotFunctionString(function));
        m_ThreadLoop->Start(m_GameConstsCache.TICK_DELAY_MICRO);
    }

    void Task::Cancel()
    {
        DE_TRACE("Stopping Task #{0} (Function:{1})", id, getRobotFunctionString(function));

        m_ThreadLoop->Stop();
    }

    bool Task::OnThreadLoopStarted()
    {
        m_ELMutex.Lock();
        {
            m_EventListener->On<EH_TaskStarted>(*this);
        }
        m_ELMutex.Unlock();
        return false;
    }
    
    bool Task::OnThreadLoopEnded(ThreadEndedReason::ThreadEndedReason_t reason)
    {
        m_ELMutex.Lock();
        if (m_EventListener != nullptr)
        {
            switch (reason)
            {
                case ThreadEndedReason::FORCED_STOP:
                    m_EventListener->On<EH_TaskCancelled>(*this);
                    break;
                case ThreadEndedReason::FINISHED:
                    m_EventListener->On<EH_TaskFinished>(*this);
                    break;
            }
        }
        m_ELMutex.Unlock();

        return false;
    }

} // namespace Application