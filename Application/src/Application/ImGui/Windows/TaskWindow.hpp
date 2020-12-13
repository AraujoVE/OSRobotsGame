#include "Window.hpp"

#include "imgui.h"

#include <memory>
#include <vector>
#include "Application/header/Task.hpp"
namespace Application
{
    class TaskWindow final : public Window
    {
    public:
        TaskWindow(std::shared_ptr<Task> task)
            : Window(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize),
              m_Task(task)
        {
        }

        virtual void Render() override
        {
            Window::SetNextPos(300, 80);
            Window::SetNextSize(200, 100);

            auto task = m_Task.lock();

            ImGui::Begin("Task", NULL, m_WindowFlags);
            {
                int currRobotsNo = task->getRobotsNo();
                int deltaRobots = 0;
                ImGui::Text("Task 1");
                ImGui::SameLine(60);
                ImGui::Button("x");
                deltaRobots += -ImGui::Button("-");
                ImGui::SameLine(30);
                deltaRobots += ImGui::Button("+");
                if (deltaRobots != 0)
                    task->setRobotsNo(currRobotsNo + deltaRobots);
            }
            ImGui::End();
        }

    private:
        std::weak_ptr<Task> m_Task;
    };
} // namespace Application