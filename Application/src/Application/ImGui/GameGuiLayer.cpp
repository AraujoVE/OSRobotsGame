#include "Application/ImGui/GameGuiLayer.hpp"

#include "mypch.hpp"

#include "Application/ImGui/GameWindows/FunctionWindow.hpp"
#include "Application/ImGui/GameWindows/StatusWindow.hpp"
#include "Application/ImGui/GameWindows/RobotCreationWindow.hpp"

namespace Application
{
    GameGuiLayer::GameGuiLayer(GameSave &gameSave)
        : m_GameRunner(std::make_shared<GameSave>())
    {
        m_StatusWindow = new StatusWindow(gameSave.GetVillageStats());

        m_FunctionWindows[(int)RobotFunction::HUNT] = new FunctionWindow(gameSave.GetRobotsManagement(), RobotFunction::HUNT);
        m_FunctionWindows[(int)RobotFunction::MEDICINE] = new FunctionWindow(gameSave.GetRobotsManagement(), RobotFunction::MEDICINE);
        m_FunctionWindows[(int)RobotFunction::CONSTRUCTION] = new FunctionWindow(gameSave.GetRobotsManagement(), RobotFunction::CONSTRUCTION);
        m_FunctionWindows[(int)RobotFunction::PROTECTION] = new FunctionWindow(gameSave.GetRobotsManagement(), RobotFunction::PROTECTION);
        m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING] = new FunctionWindow(gameSave.GetRobotsManagement(), RobotFunction::RESOURCE_GATHERING);

        m_RobotCreationWindow = new RobotCreationWindow(gameSave.GetRobotsManagement());

        m_GameRunner.setOnGameStarted(new EH_GameStarted([=](GameRunner& gameRunner) {
            m_FunctionWindows[(int)RobotFunction::HUNT]->SetEventHandlers(gameRunner.GetSave().GetRobotsManagement());
            m_FunctionWindows[(int)RobotFunction::MEDICINE]->SetEventHandlers(gameRunner.GetSave().GetRobotsManagement());
            m_FunctionWindows[(int)RobotFunction::CONSTRUCTION]->SetEventHandlers(gameRunner.GetSave().GetRobotsManagement());
            m_FunctionWindows[(int)RobotFunction::PROTECTION]->SetEventHandlers(gameRunner.GetSave().GetRobotsManagement());
            m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING]->SetEventHandlers(gameRunner.GetSave().GetRobotsManagement());
            return false;
        }));

        m_GameRunner.Start();

        // m_scriptLoop = new EAScript(m_GameSave, m_FunctionWindows, m_RobotCreationWindow, "gameScript.cfg");
    }

    void GameGuiLayer::ImGuiDescription()
    {
        if (m_GameRunner.IsGameLost()) {
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
    }

    void GameGuiLayer::LostScreenDescription() {
        bool restart, quit;
        const static auto xSize = 300u, ySize = 125u;
        ImGui::SetNextWindowPos(
            {
                ImGui::GetMainViewport()->Pos.x + (ImGui::GetMainViewport()->Size.x - xSize) /2, 
                ImGui::GetMainViewport()->Pos.y + (ImGui::GetMainViewport()->Size.y - ySize ) /2
            });
        ImGui::SetNextWindowSize({xSize, ySize});
        ImGui::Begin("Game Over", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            //TODO!!
            ImGui::Text("%s", "TODO!! lost message");

            ImGui::Text("Do you want to play again?");

            restart =  ImGui::Button("Yes"); 
            ImGui::SameLine();
            quit = ImGui::Button("No");

        }
        ImGui::End();

        if (quit) DampEngine::Application::GetCurrent().Stop();
        else if (restart) {
            for (int i = 0; i < FUNCTION_QTY; i++)
                m_FunctionWindows[i]->ClearTaskWindows();

            m_GameRunner.Start();
        }

    }
} // namespace Application