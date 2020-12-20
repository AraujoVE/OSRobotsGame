#include "MainGuiLayer.hpp"

#include "mypch.hpp"

#include "Application/ImGui/Window/FunctionWindow.hpp"
#include "Application/ImGui/Window/StatusWindow.hpp"
#include "Application/ImGui/Window/RobotCreationWindow.hpp"

namespace Application
{
    MainGuiLayer::MainGuiLayer(GameSave &gameSave)
        : m_GameSave(gameSave)
    {
        m_StatusWindow = new StatusWindow(gameSave.getVillageStats());

        m_FunctionWindows[(int)RobotFunction::HUNT] = new FunctionWindow(gameSave.getRobotsManagement(), RobotFunction::HUNT);
        m_FunctionWindows[(int)RobotFunction::MEDICINE] = new FunctionWindow(gameSave.getRobotsManagement(), RobotFunction::MEDICINE);
        m_FunctionWindows[(int)RobotFunction::CONSTRUCTION] = new FunctionWindow(gameSave.getRobotsManagement(), RobotFunction::CONSTRUCTION);
        m_FunctionWindows[(int)RobotFunction::PROTECTION] = new FunctionWindow(gameSave.getRobotsManagement(), RobotFunction::PROTECTION);
        m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING] = new FunctionWindow(gameSave.getRobotsManagement(), RobotFunction::RESOURCE_GATHERING);

        gameSave.getRobotsManagement()->setFunctionWindowsArray(m_FunctionWindows);

        m_RobotCreationWindow = new RobotCreationWindow(gameSave.getRobotsManagement());

        m_GameLost = false;
    }
    

    void MainGuiLayer::ImGuiDescription()
    {
        if (m_GameLost) {
            LostScreenDescription();
            return;
        }

        ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None;

        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoResize;

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y));
        ImGui::SetNextWindowSize(ImVec2(main_viewport->GetWorkSize().x, 40));

        ImGui::Begin("Status", NULL, windowFlags);
        {
            ImGui::Text("Status");
        }
        ImGui::End();

        for (int i = 0; i < FUNCTION_QTY; i++)
        {
            m_FunctionWindows[i]->Render();
        }

        m_StatusWindow->Render();
        m_RobotCreationWindow->Render();

        //Check if game is lost
        if (m_GameSave.getVillageStats()->getPopulation() <= 0)
            m_GameLost = true;
    }

    void MainGuiLayer::LostScreenDescription() {
        bool restart, quit;
        ImGui::Begin("Game Over", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::Text("Your population reached 0!!");
            ImGui::Text("Do you want to play again?");
            restart = ImGui::Button("Yes"); ImGui::SameLine(); quit = ImGui::Button("No");
        }
        ImGui::End();

        if (quit) DampEngine::Application::GetCurrent().Stop();
        else if (restart) {
            m_GameLost = false;
            for (int i = 0; i < FUNCTION_QTY; i++)
                m_FunctionWindows[i]->ClearTaskWindows();
            m_GameSave.clear();
            m_GameSave.getRobotsManagement()->setFunctionWindowsArray(m_FunctionWindows);
        }

    }
} // namespace Application