#include "Application/header/Task.hpp"
#include "Application/header/ConstsMap.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include "DampEngine/Core/Macros/Log.hpp"

#include "mypch.hpp"

namespace Application
{
    //Inicialização e destruição de classe
    Task::Task(RobotFunction funct, GameConstsCache& gameConsts, Task::TaskID id)
        : m_GameConstsCache(gameConsts),
          m_ThreadLoop(std::bind(&Task::UpdateTask, this), std::bind([](Task *self) { return !self->IsTaskCompleted(); }, this)),
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

        m_Running = false;

        m_EventListener.reset(new EventListener());

        m_ThreadLoop.m_EventListener->Register(new EH_ThreadStarted(std::bind(&Task::OnThreadLoopStarted, this)));
        m_ThreadLoop.m_EventListener->Register(new EH_ThreadEnded(std::bind(&Task::OnThreadLoopEnded, this, std::placeholders::_1)));

    }

    Task::~Task()
    {
        m_Running = false;
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
        m_ThreadLoop.Start();
    }

    void Task::Cancel()
    {
        DE_TRACE("Stopping Task #{0} (Function:{1})", id, getRobotFunctionString(function));
        
        m_ThreadLoop.Stop();
    }

    void Task::MarkAsIgnored()
    {
        DE_TRACE("Detaching Task #{0} (Function:{1})", id, getRobotFunctionString(function));
        m_ThreadLoop.m_EventListener->Clear();
        m_ThreadLoop.Stop();
    }

    bool Task::OnThreadLoopStarted()
    {
        m_EventListener->On<EH_TaskStarted>(*this);
        return true;
    }
    bool Task::OnThreadLoopEnded(ThreadEndedReason::ThreadEndedReason_t reason)
    {
        switch (reason) {
            case ThreadEndedReason::STOP: m_EventListener->On<EH_TaskCancelled>(*this); break;
            case ThreadEndedReason::FINISHED: m_EventListener->On<EH_TaskFinished>(*this); break;
        }
        
        return false;
    }

} // namespace Application