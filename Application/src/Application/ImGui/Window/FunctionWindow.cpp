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
        pthread_mutex_init(&m_MutexMapRemoval, NULL);
    }

    void FunctionWindow::Render()
    {
        this->SetNextPos();
        this->SetNextSize();

        //New thread button control variables
        bool createNewTaskIssued = false;
        bool canCreateNewTask = (m_TaskWindowMap.size() < m_RobotsManagement.MAX_TASKS_PER_FUNCTION);

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


        //TODO: create Update() method
        
        pthread_mutex_lock(&m_MutexMapRemoval);
        while (!m_TasksPendingDeletion.empty()) {
            TaskID toDeleteID = m_TasksPendingDeletion.front();
            auto pendingIt = m_TaskWindowMap.find(toDeleteID);
            DE_ASSERT(pendingIt != m_TaskWindowMap.end(), "Invalid task (not in map)");
            auto window =  pendingIt->second;
            delete window;
            m_TaskWindowMap.erase(pendingIt);
            m_TasksPendingDeletion.pop();
        }
        pthread_mutex_unlock(&m_MutexMapRemoval);


        //Render children
        for (auto taskWindowPair : m_TaskWindowMap)
            taskWindowPair.second->Render();
    }

    void FunctionWindow::CreateTask()
    {
        auto taskCanceledCallback = std::bind(&RobotsManagement::endTask, m_RobotsManagement, std::placeholders::_1);

        //TODO: make m_RobotsManagement return the task ID, to keep track internally
        Task &createdTask = m_RobotsManagement.createTask(m_Function);

        TaskWindowProps associatedWindowProps = {m_TaskWindowMap.size(), m_WindowProps};
        TaskWindow *associatedWindow = new TaskWindow(associatedWindowProps, m_RobotsManagement, createdTask, taskCanceledCallback);

        m_TaskWindowMap.insert({createdTask.getId(), associatedWindow});
    }

    void FunctionWindow::OnTaskEnded(Task &endedTask)
    {
        DE_DEBUG("(FunctionWindow) onTaskEnded()");

        auto windowIt = m_TaskWindowMap.find(endedTask.getId());
        unsigned long windowIndex = windowIt->second->GetIndex();

        DE_ASSERT(windowIt != m_TaskWindowMap.end(), "Trying to end an unknown Task");
        
        pthread_mutex_lock(&m_MutexMapRemoval);
        for (auto affectedPair : m_TaskWindowMap)
        {
            if (affectedPair.second->GetIndex() > windowIndex)
                affectedPair.second->SetIndex(affectedPair.second->GetIndex()-1);
        }
        pthread_mutex_unlock(&m_MutexMapRemoval);
        
        
        DE_DEBUG("INSERINDO O {0}", endedTask.getId());
        m_TasksPendingDeletion.push(endedTask.getId());

    }
} // namespace Application