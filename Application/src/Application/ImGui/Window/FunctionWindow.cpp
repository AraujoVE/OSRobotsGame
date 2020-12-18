#include "FunctionWindow.hpp"

#include "mypch.hpp"

#include "Application/ImGui/Window/TaskWindow.hpp"

namespace Application
{

    FunctionWindow::FunctionWindow(RobotsManagement &robotsManagement, RobotFunction function)
        : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize),
          m_Function(function), m_RobotsManagement(robotsManagement)
    {
        //Next window position and size
        const static float ySpacing = 10;
        const static float ySize = 100;
        m_WindowProps = {
            {40, 80 + (ySize + ySpacing) * (int)function},
            {200, ySize}};
    }

    void FunctionWindow::Render()
    {
        this->SetNextPos();
        this->SetNextSize();

        //New thread button control variables
        bool createNewTaskIssued = false;
        bool canCreateNewTask = (m_TaskWindows.size() < m_RobotsManagement.MAX_TASKS_PER_FUNCTION);

        //Window description
        ImGui::Begin(("Function_" + getRobotFunctionString(m_Function)).c_str(), NULL, m_WindowFlags);
        {
            ImGui::Text("%s", getRobotFunctionString(m_Function).c_str());
            ImGui::SameLine();

            if (canCreateNewTask)
            {
                createNewTaskIssued = ImGui::ButtonEx("+", {0, 0}, ImGuiButtonFlags_Repeat);
            }
            else
            {
                //If it cannot create more tasks, the button should be disabled
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                createNewTaskIssued = ImGui::ButtonEx("+", {0, 0}, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_Disabled);
                ImGui::PopStyleVar();
            }
        }
        ImGui::End();

        //Task creation if issued
        if (createNewTaskIssued && canCreateNewTask)
            CreateTask();

        //Render children
        for (auto taskWindow : m_TaskWindows)
            taskWindow->Render();
    }

    void FunctionWindow::CreateTask()
    {
        auto taskCanceledCallback = std::bind(&FunctionWindow::EndTask, this, std::placeholders::_1);

        //TODO: make m_RobotsManagement return the task ID, to keep track internally
        Task &createdTask = m_RobotsManagement.createTask(m_Function);

        TaskWindowProps associatedWindowProps = {m_TaskWindows.size(), m_WindowProps};
        TaskWindow *associatedWindow = new TaskWindow(associatedWindowProps, m_RobotsManagement, createdTask, taskCanceledCallback);

        m_TaskWindows.push_back(associatedWindow);
    }

    void FunctionWindow::EndTask(TaskWindow *taskWindow)
    {
        //TODO: m_RobotsManagement.endTask(taskWindow->GetTaskID());

        unsigned long deletedWindowIndex = taskWindow->GetIndex();

        m_TaskWindows.erase(m_TaskWindows.begin() + deletedWindowIndex);

        for (unsigned long affectedWindowOldIndex = deletedWindowIndex; affectedWindowOldIndex < m_TaskWindows.size(); affectedWindowOldIndex++)
        {
            m_TaskWindows[affectedWindowOldIndex]->SetIndex(affectedWindowOldIndex + 1);
        }

        delete taskWindow;
    }
} // namespace Application