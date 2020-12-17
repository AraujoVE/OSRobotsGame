#pragma once

#include "IGWindow.hpp"

#include "imgui_internal.h"
#include "imgui.h"

#include "Application/header/RobotFunctions.hpp"
#include "Application/header/RobotsManagement.hpp"

#include "TaskWindow.hpp"

#include <vector>
#include <algorithm>

namespace Application
{
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