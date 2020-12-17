#include "TaskWindow.hpp"

#include "DampEngine/Core/Macros.hpp"

namespace Application
{

    TaskWindow::TaskWindow(TaskWindowProps taskWindowProps, RobotsManagement &robotsManagement, TaskID taskID, OnTaskCancelledFn onTaskCancelledFn)
        : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings),
          m_TaskWindowProps(taskWindowProps),
          m_RobotsManagement(robotsManagement), m_TaskID(taskID),
          //TODO: include RobotFunction info in title
          m_WindowName(std::string("Task_") + std::to_string(taskID)),
          m_OnTaskCancelledFn(onTaskCancelledFn)
    {
        UpdateProps();
    }

    void TaskWindow::Render()
    {
        SetNextPos();
        SetNextSize();

        // Task& task = m_RobotsManagement.findTask(m_TaskID);

        bool cancelIssued = false;
        int deltaRobots = 0;

        ImGui::Begin(m_WindowName.c_str(), NULL, m_WindowFlags);
        {
            // int currRobotsNo = task.getRobotsNo();
            ImGui::Text("Task 1");
            ImGui::SameLine(60);
            cancelIssued = ImGui::Button("x");
            deltaRobots += -ImGui::Button("-");
            ImGui::SameLine(30);
            deltaRobots += ImGui::Button("+");
        }
        ImGui::End();

        if (cancelIssued)
        {
            m_OnTaskCancelledFn(this);
        }
        else if (deltaRobots != 0)
            DE_DEBUG("Delta = {0}", deltaRobots);
        //     task.setRobotsNo(currRobotsNo + deltaRobots);
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
