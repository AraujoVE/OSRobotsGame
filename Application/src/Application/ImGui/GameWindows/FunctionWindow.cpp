#include "FunctionWindow.hpp"

#include "mypch.hpp"

#include "Application/ImGui/GameWindows/TaskWindow.hpp"
#include "Application/Game/Ingame/RobotsManagement.hpp"

using namespace Application;
namespace Application::GameWindows
{
    FunctionWindow::FunctionWindow(RobotsManagement* robotsManagement, RobotFunction function)
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
            m_RobotsManagement->createTask(m_Function);

        //Render children
        m_TaskWindowMapMutex.Lock();
        {
            for (auto taskWindowPairIt = m_TaskWindowMap.begin(); taskWindowPairIt != m_TaskWindowMap.end(); taskWindowPairIt++)
                taskWindowPairIt->second->Render();
        }
        m_TaskWindowMapMutex.Unlock();
    }

    void FunctionWindow::SetEventHandlers(RobotsManagement *robotsManagement)
    {
        RobotFunction thisFunction = m_Function;
        robotsManagement->setOnTaskCreated(new EH_TaskCreated([=](Task &createdTask) {
            if (createdTask.GetRobotFunction() != thisFunction)
                return false;

            m_TaskWindowMapMutex.Lock();
            {
                auto taskCanceledCallback = std::bind(&RobotsManagement::cancelTask, m_RobotsManagement, std::placeholders::_1);
                TaskWindowProps associatedWindowProps = {m_TaskWindowMap.size(), m_WindowProps};
                TaskWindow *associatedWindow = new TaskWindow(associatedWindowProps, m_RobotsManagement, createdTask, taskCanceledCallback);

                m_TaskWindowMap.insert({createdTask.GetID(), associatedWindow});
            }
            m_TaskWindowMapMutex.Unlock();

            return true;
        }));

        auto removeTaskFromGUI = [=](Task &endedTask) {
            if (endedTask.GetRobotFunction() != thisFunction)
                return false;

            this->OnTaskEnded(endedTask);
            return true;
        };

        robotsManagement->setOnTaskEnded(new EH_TaskFinished(removeTaskFromGUI));
        robotsManagement->setOnTaskCancelled(new EH_TaskCancelled(removeTaskFromGUI));
    }

    void FunctionWindow::ClearTaskWindows()
    {
        m_TaskWindowMapMutex.Lock();
        {
            for (auto deletePair : m_TaskWindowMap)
            {
                delete deletePair.second;
            }
            m_TaskWindowMap.clear();
        }
        m_TaskWindowMapMutex.Unlock();
    }

    void FunctionWindow::OnTaskEnded(Task &endedTask)
    {
        OnTaskEnded(endedTask.GetID());
    }

    void FunctionWindow::OnTaskEnded(int id)
    {
        m_TaskWindowMapMutex.Lock();
        {
            auto windowIt = m_TaskWindowMap.find(id);

            if (windowIt == m_TaskWindowMap.end()) {
                DE_WARN("(FunctionWindow::OnTaskEnded) Trying to end an unknown Task... Ignoring.\n\t (TODO: Make EAController wait for UI if ShowGame is on)");
                m_TaskWindowMapMutex.Unlock();
                return;
            }

            m_TaskWindowMap.erase(windowIt);

            int i = m_TaskWindowMap.size() - 1;
            for (windowIt = m_TaskWindowMap.begin(); windowIt != m_TaskWindowMap.end(); windowIt++)
            {
                (windowIt->second)->SetIndex(i--);
            }
        }
        m_TaskWindowMapMutex.Unlock();
    }

} // namespace Application::GameWindows