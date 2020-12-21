#include "FunctionWindow.hpp"

#include "mypch.hpp"

#include "Application/ImGui/Window/TaskWindow.hpp"

namespace Application
{
    FunctionWindow::FunctionWindow(std::unique_ptr<RobotsManagement> &robotsManagement, RobotFunction function)
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
        bool canCreateNewTask = (m_TaskWindowMap.size() < m_RobotsManagement->MAX_TASKS_PER_FUNCTION);

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
        for (auto taskWindowPair : m_TaskWindowMap)
            taskWindowPair.second->Render();
    }

    void FunctionWindow::CreateTask()
    {
        auto taskCanceledCallback = std::bind(&RobotsManagement::endTask, m_RobotsManagement.get(), std::placeholders::_1);

        
        Task &createdTask = m_RobotsManagement->createTask(m_Function);

        TaskWindowProps associatedWindowProps = {m_TaskWindowMap.size(), m_WindowProps};
        TaskWindow *associatedWindow = new TaskWindow(associatedWindowProps, m_RobotsManagement, createdTask, taskCanceledCallback);

        m_TaskWindowMap.insert({createdTask.getId(), associatedWindow});
    }


    void FunctionWindow::ClearTaskWindows() {
        pthread_mutex_lock(&m_MutexMapRemoval);
        
        for (auto deletePair : m_TaskWindowMap)
        {
            DE_DEBUG("Deleting TaskWindow ID={0}, Index={1}, Function={2}", deletePair.first, deletePair.second->GetIndex(), getRobotFunctionString(deletePair.second->GetTask().getType()));
            delete deletePair.second;
        }
        m_TaskWindowMap.clear();
        pthread_mutex_unlock(&m_MutexMapRemoval);
    }

    void FunctionWindow::OnTaskEnded(Task &endedTask)
    {
        DE_DEBUG("(FunctionWindow) onTaskEnded()");

        auto windowIt = m_TaskWindowMap.find(endedTask.getId());

        DE_ASSERT(windowIt != m_TaskWindowMap.end(), "Trying to end an unknown Task");

        pthread_mutex_lock(&m_MutexMapRemoval);

        m_TaskWindowMap.erase(windowIt);
        
        int i = m_TaskWindowMap.size()-1;
        for (windowIt = m_TaskWindowMap.begin() ; windowIt != m_TaskWindowMap.end() ; windowIt++)
        {
            (windowIt->second)->SetIndex(i--); 
        }

        pthread_mutex_unlock(&m_MutexMapRemoval);
        
        
        DE_DEBUG("INSERINDO O {0}", endedTask.getId());
        m_TasksPendingDeletion.push(endedTask.getId());

    }
} // namespace Application