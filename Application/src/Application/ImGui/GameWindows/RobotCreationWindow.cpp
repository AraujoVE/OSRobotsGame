#include "RobotCreationWindow.hpp"

#include "Application/header/RobotsManagement.hpp"

#include "mypch.hpp"

using namespace Application;
namespace Application::GameWindows
{
    void RobotCreationWindow::Render()
    {
        this->SetNextPos();
        this->SetNextSize();

        bool createRobot = false;
        bool destroyRobot = false;

        bool canCreateRobot = m_RobotsManagement->canAddRobots();
        bool canDestroyRobot = m_RobotsManagement->canRemoveRobots();

        ImGui::Begin("Robot management", NULL, m_WindowFlags);
        {  
            //First line
            ImGui::Text("Total Robot count: %d", m_RobotsManagement->getTotRobots());
            ImGui::Text("Free Robots: %d/%d", m_RobotsManagement->getFreeRobots() , m_RobotsManagement->getTotRobots());

            //Third line

            //TODO: remove robs and diable if cant
            ImGui::Text("Production cost: %d", m_RobotsManagement->getProdCost());

            if (!canCreateRobot) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                createRobot = ImGui::ButtonEx("Create robot", {0,0}, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_Disabled);
                ImGui::PopStyleVar();
                
            } else createRobot = ImGui::ButtonEx("Create robot", {0,0}, ImGuiButtonFlags_Repeat); 
            
            if (!canDestroyRobot) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                destroyRobot = ImGui::ButtonEx("Destroy robot", {0,0}, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_Disabled);
                ImGui::PopStyleVar();
            } else destroyRobot = ImGui::ButtonEx("Destroy robot", {0,0}, ImGuiButtonFlags_Repeat); 
        }

        if (createRobot && canCreateRobot) {
            m_RobotsManagement->createRobots(1);
        } else if (destroyRobot && canDestroyRobot) {
            m_RobotsManagement->destroyRobots(1);
        }
        ImGui::End();
    }

} // namespace Application
