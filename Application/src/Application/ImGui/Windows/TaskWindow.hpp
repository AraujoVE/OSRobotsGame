#include "IGWindow.hpp"

#include "imgui.h"

#include <memory>
#include <vector>
#include "Application/header/Task.hpp"

#include "DampEngine/Core/Macros.hpp"

namespace Application
{
    class TaskWindow final : public Application::IGWindow
    {
    public:
        TaskWindow(RobotsManagement& robotsManagement, TaskID taskID)
            : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize),
            m_RobotsManagement(robotsManagement), m_TaskID(taskID)
        {
        }

        virtual void Render() override
        {
            IGWindow::SetNextPos(300, 80);
            IGWindow::SetNextSize(200, 100);


            Task& task = m_RobotsManagement.findTask(m_TaskID);

            ImGui::Begin("Task", NULL, m_WindowFlags);
            {
                int currRobotsNo = task.getRobotsNo();
                int deltaRobots = 0;
                ImGui::Text("Task 1");
                ImGui::SameLine(60);
                ImGui::Button("x");
                deltaRobots += -ImGui::Button("-");
                ImGui::SameLine(30);
                deltaRobots += ImGui::Button("+");
                if (deltaRobots != 0)
                    task.setRobotsNo(currRobotsNo + deltaRobots);
            }
            ImGui::End();
        }

    private:
        RobotsManagement &m_RobotsManagement;
        TaskID m_TaskID;
    };
} // namespace Application