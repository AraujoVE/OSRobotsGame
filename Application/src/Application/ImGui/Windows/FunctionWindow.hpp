#pragma once

#include "IGWindow.hpp"

#include "imgui_internal.h"
#include "imgui.h"

#include "Application/header/RobotFunctions.hpp"
#include "Application/header/RobotsManagement.hpp"

#include "TaskWindow.hpp"

#include <vector>

namespace Application
{    class FunctionWindow final : public Application::IGWindow
    {
    public:
        FunctionWindow(RobotsManagement &robotsManagement, RobotFunction function)
            : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize),
              m_Function(function), m_RobotsManagement(robotsManagement)
        { 
        }

        virtual void Render() override
        {
            bool createNewTaskIssued = false;
            bool canCreateNewTask = (m_TaskWindows.size() < m_RobotsManagement.MAX_TASKS_PER_FUNCTION);

            const static float ySize = 100;
            const static float ySpacing = 10;

            this->SetNextPos(40, 80 + (ySize + ySpacing) * (int)m_Function);

            this->SetNextSize(200, ySize);
            ImGui::Begin(("Function_"+RobotFunctionString[(int)m_Function]).c_str(), NULL, m_WindowFlags);
            {
                ImGui::Text("%s",RobotFunctionString[(int)m_Function].c_str());
                ImGui::SameLine();

                createNewTaskIssued = ImGui::ButtonEx("+", {0,0}, canCreateNewTask ? (int)ImGuiButtonFlags_None : (int)ImGuiButtonFlags_Disabled);
            }
            ImGui::End();

            //TODO: make m_RobotsManagement return the task ID, to keep track internally
            if (createNewTaskIssued) m_RobotsManagement.createTask(m_Function);
        }
    private:
        RobotFunction m_Function;
        RobotsManagement &m_RobotsManagement;
        std::vector<TaskWindow*> m_TaskWindows;
    };
} // namespace Application