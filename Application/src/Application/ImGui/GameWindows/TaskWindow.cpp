#include "Application/ImGui/GameWindows/TaskWindow.hpp"

#include "Application/header/RobotsManagement.hpp"

#include "mypch.hpp"

namespace Application
{

    TaskWindow::TaskWindow(TaskWindowProps taskWindowProps, std::unique_ptr<RobotsManagement> &robotsManagement, Task &task, OnTaskCancelledFn onTaskCancelledFn)
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
            ImGui::SameLine(m_WindowProps.size.x-20);
            cancelIssued = ImGui::Button("x");

            //Second line
            ImGui::Text("Robot count: %d", m_Task.getRobotsNo());

            //Third line
            deltaRobots += -ImGui::ButtonEx("-", {0,0}, ImGuiButtonFlags_Repeat);
            ImGui::SameLine(30);
            deltaRobots += +ImGui::ButtonEx("+", {0,0}, ImGuiButtonFlags_Repeat);
            ImGui::SameLine(110);
            ImGui::Text("Goods: %.2f", m_Task.getGainedGoods());
            time_t remainingTime = m_Task.getRemainingTime();

            const char *endTimeStr = "--";
            if (remainingTime >= 0) {
                std::stringstream ss;
                ss << remainingTime << "s";
                endTimeStr = ss.str().c_str();
            }

            ImGui::Text("end time: %s", endTimeStr);
        }
        ImGui::End();

        if (cancelIssued)
        {
            //This callback destroys the current object
            DE_DEBUG("(click) Pedindo para parar task {0}", m_Task.getId());
            m_OnTaskCancelledFn(m_Task);
            return;
        }

        if (deltaRobots != 0)
            m_RobotsManagement->moveRobot(m_Task, deltaRobots);
    }

    void TaskWindow::UpdateProps()
    {
        constexpr static float padding = 0;

        m_WindowName = std::string("Task (") + getRobotFunctionString(m_Task.getType()) + ") #" + std::to_string(m_Task.getId());  

        DE_ASSERT(m_TaskWindowProps.ParentProps.size.y >= padding, "Padding is greater than root window size");

        constexpr static float xSize = 250;
        float ySize = m_TaskWindowProps.ParentProps.size.y - padding;

        float xPos = (m_TaskWindowProps.FunctionTaskVecIdx * (xSize + m_TaskWindowProps.xSpacing) +
                      m_TaskWindowProps.ParentProps.pos.x + m_TaskWindowProps.ParentProps.size.x +
                      m_TaskWindowProps.xOffsetFirst + padding);

        float yPos = m_TaskWindowProps.ParentProps.pos.y + padding;

        m_WindowProps = {
            {xPos, yPos},
            {xSize, ySize}};
    }

    Task& TaskWindow::getTask(){
        return m_Task;
    }

} // namespace Application
