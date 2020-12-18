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

    private:
        void CreateTask();
        void EndTask(TaskWindow *taskWindow);

    private:
        RobotFunction m_Function;
        RobotsManagement &m_RobotsManagement;
        std::vector<TaskWindow *> m_TaskWindows;
    };
} // namespace Application