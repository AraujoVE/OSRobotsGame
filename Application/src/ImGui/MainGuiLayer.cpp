#include "MainGuiLayer.hpp"
#include "imgui.h"

void MainGuiLayer::ImGuiDescription() {

    ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None;

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

    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 40, main_viewport->GetWorkPos().y + 80));
    ImGui::SetNextWindowSize(ImVec2(200, 100));
    ImGui::Begin("Function", NULL, windowFlags);
    {
        ImGui::Text("Function 1"); ImGui::SameLine(100); ImGui::Button("+");
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 300, main_viewport->GetWorkPos().y + 80));
    ImGui::SetNextWindowSize(ImVec2(200, 100));
    ImGui::Begin("Task", NULL, windowFlags);
    {
        ImGui::Text("Task 1"); ImGui::SameLine(60); ImGui::Button("x");
        ImGui::Button("-"); ImGui::SameLine(30); ImGui::Button("+");
    }
    ImGui::End();
}   
