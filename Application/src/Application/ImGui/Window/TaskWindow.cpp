#include "TaskWindow.hpp"

#include "DampEngine/Core/Macros.hpp"

namespace Application
{

    TaskWindow::TaskWindow(TaskWindowProps taskWindowProps, RobotsManagement &robotsManagement, Task &task, OnTaskCancelledFn onTaskCancelledFn)
        : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings),
          m_TaskWindowProps(taskWindowProps),
          m_RobotsManagement(robotsManagement), m_Task(task),
          m_WindowName(std::string("Task_") + std::to_string(task.getId())),
          m_OnTaskCancelledFn(onTaskCancelledFn)
    {
        UpdateProps();
    }

    void TaskWindow::Render()
    {
        SetNextPos();
        SetNextSize();

        bool cancelIssued = false;
        int deltaRobots = 0;

        ImGui::Begin(m_WindowName.c_str(), NULL, m_WindowFlags);
        {  
            //First line
            ImGui::Text("%s",m_WindowName.c_str());
            ImGui::SameLine(60);
            cancelIssued = ImGui::Button("x");

            //Second line
            ImGui::Text("Robot count: %d", m_Task.getRobotsNo());

            //Third line
            deltaRobots += -ImGui::Button("-");
            ImGui::SameLine(30);
            deltaRobots += +ImGui::Button("+");
            ImGui::SameLine(110);
            ImGui::Text("Goods: %.2f", m_Task.getGainedGoods());
            
        }
        ImGui::End();

        if (cancelIssued)
        {
            //This callback destroys the current object
            m_OnTaskCancelledFn(this);
            return;
        }


        if (deltaRobots != 0)
            m_RobotsManagement.moveRobot(m_Task, deltaRobots);
    }

    void TaskWindow::UpdateProps()
    {
        constexpr static float padding = 0;

        DE_ASSERT(m_TaskWindowProps.ParentProps.size.y >= padding, "Padding is greater than root window size");

        constexpr static float xSize = 200;
        float ySize = m_TaskWindowProps.ParentProps.size.y - padding;

        float xPos = (m_TaskWindowProps.FunctionTaskVecIdx * (xSize + m_TaskWindowProps.xSpacing) +
                      m_TaskWindowProps.ParentProps.pos.x + m_TaskWindowProps.ParentProps.size.x +
                      m_TaskWindowProps.xOffsetFirst + padding);

        float yPos = m_TaskWindowProps.ParentProps.pos.y + padding;

        m_WindowProps = {
            {xPos, yPos},
            {xSize, ySize}};
    }

} // namespace Application
