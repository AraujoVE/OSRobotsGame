#include "MainGuiLayer.hpp"
#include "imgui.h"

#include "DampEngine/Core/Macros/Log.hpp"

MainGuiLayer::MainGuiLayer() {
    m_FunctionWindow.reset(new FunctionWindow(RobotFunction::PROTECTION));
}

void MainGuiLayer::ImGuiDescription() {
    ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None;

    DE_INFO("Task id={0} has {1} robots", m_Task->getId(), m_Task->getRobotsNo());

    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoResize;

    this->ImGuiLayer::ImGuiDescription();

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y ));
    ImGui::SetNextWindowSize(ImVec2(main_viewport->GetWorkSize().x, 40));
    
    ImGui::Begin("Status", NULL, windowFlags);
    {
        ImGui::Text("Status");
    }
    ImGui::End();

    
}   
