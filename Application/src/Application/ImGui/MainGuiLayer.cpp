#include "MainGuiLayer.hpp"
#include "imgui.h"

#include "DampEngine/Core/Macros/Log.hpp"
namespace Application
{
    MainGuiLayer::MainGuiLayer(GameSave &gameSave)
            : m_GameSave(gameSave)
        {
            m_FunctionWindows[(int)RobotFunction::HUNT] = new FunctionWindow(gameSave.getRobotManagement(), RobotFunction::HUNT);
            m_FunctionWindows[(int)RobotFunction::MEDICINE] = new FunctionWindow(gameSave.getRobotManagement(), RobotFunction::MEDICINE);
            m_FunctionWindows[(int)RobotFunction::CONSTRUCTION] = new FunctionWindow(gameSave.getRobotManagement(), RobotFunction::CONSTRUCTION);
            m_FunctionWindows[(int)RobotFunction::PROTECTION] = new FunctionWindow(gameSave.getRobotManagement(), RobotFunction::PROTECTION);
            m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING] = new FunctionWindow(gameSave.getRobotManagement(), RobotFunction::RESOURCE_GATHERING);
        }

    void MainGuiLayer::ImGuiDescription()
    {
        ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None;

        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoResize;

        this->ImGuiLayer::ImGuiDescription();

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y));
        ImGui::SetNextWindowSize(ImVec2(main_viewport->GetWorkSize().x, 40));

        ImGui::Begin("Status", NULL, windowFlags);
        {
            ImGui::Text("Status");
        }
        ImGui::End();

            for (int i = 0; i < RobotsManagement::FUNCTION_QTY; i++)
            {
                m_FunctionWindows[i]->Render();
            }        

    }
} // namespace Application