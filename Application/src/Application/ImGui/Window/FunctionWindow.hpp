#pragma once

#include "IGWindow.hpp"

namespace Application
{
    class TaskWindow;
    class RobotsManagement;

    class FunctionWindow final : public Application::IGWindow
    {
    public:
        FunctionWindow(RobotsManagement &robotsManagement, RobotFunction function);

        virtual void Render() override;

        void OnTaskEnded(Task& endedTask);
    private:
        void CreateTask();

    private:
        RobotFunction m_Function;
        RobotsManagement &m_RobotsManagement;
        std::unordered_map<TaskID, TaskWindow *> m_TaskWindowMap;
        std::queue<TaskID> m_TasksPendingDeletion;
        pthread_mutex_t m_MutexMapRemoval;
    };
} // namespace Application