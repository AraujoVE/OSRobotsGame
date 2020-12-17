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
{    class FunctionWindow final : public Application::IGWindow
    {
    public:
        FunctionWindow(RobotsManagement &robotsManagement, RobotFunction function)
            : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize),
              m_Function(function), m_RobotsManagement(robotsManagement)
        {
            //Next window position and size
            const static float ySpacing = 10;
            const static float ySize = 100;
            m_WindowProps = {
                {40, 80 + (ySize + ySpacing) * (int)function },
                {200, ySize}
            };
        }

        virtual void Render() override
        {            
            this->SetNextPos();
            this->SetNextSize();

            //New thread button control variables
            bool createNewTaskIssued = false;
            bool canCreateNewTask = (m_TaskWindows.size() < m_RobotsManagement.MAX_TASKS_PER_FUNCTION);

            //Window description
            ImGui::Begin(("Function_"+RobotFunctionString[(int)m_Function]).c_str(), NULL, m_WindowFlags);
            {
                ImGui::Text("%s",RobotFunctionString[(int)m_Function].c_str());
                ImGui::SameLine();


                if (canCreateNewTask) {
                    createNewTaskIssued = ImGui::ButtonEx("+", {0,0}, ImGuiButtonFlags_Repeat);
                } else {
                    //If it cannot create more tasks, the button should be disabled 
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                    createNewTaskIssued = ImGui::ButtonEx("+", {0,0}, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_Disabled);
                    ImGui::PopStyleVar();
                }
            }
            ImGui::End();

            //Task creation if issued
            if (createNewTaskIssued && canCreateNewTask) {
                CreateTask();
            }

            //Render children
            for (auto taskWindow : m_TaskWindows) {
                taskWindow->Render();
            }
        }
    private:
        void CreateTask() {
            static int taskID = 0;
            //TODO: make m_RobotsManagement return the task ID, to keep track internally
            // m_RobotsManagement.createTask(m_Function);

            int taskIdx = m_TaskWindows.size();
            m_TaskWindows.push_back(
                new TaskWindow(
                    TaskWindowProps(taskIdx, m_WindowProps), 
                    m_RobotsManagement,
                    taskID,
                    std::bind(&FunctionWindow::EndTask, this, std::placeholders::_1)
                )
            );

            //Temporary
            taskID++;
        }

        void EndTask(TaskWindow* taskWindow) {
            //TODO: m_RobotsManagement.endTask(taskWindow->GetTaskID());
    
            auto it = std::find(m_TaskWindows.begin(), m_TaskWindows.end(), taskWindow);
            if (it != m_TaskWindows.end()) {
                m_TaskWindows.erase(it);
                for (;it != m_TaskWindows.end(); it++) {
                    (*it)->SetIndex(it - m_TaskWindows.begin());
                }
            }

            delete taskWindow;
        }

    private:
        RobotFunction m_Function;
        RobotsManagement &m_RobotsManagement;
        std::vector<TaskWindow*> m_TaskWindows;
    };
} // namespace Application