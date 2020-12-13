#include "Window.hpp"

#include "imgui.h"
#include "header/RobotFunctions.hpp"

class FunctionWindow final : public Window
{
public:
    FunctionWindow(RobotFunction function)
        : Window(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize), 
        m_Function(function)
    {
    }

    virtual void Render() override
    {

        Window::SetNextPos(40, 80);
        Window::SetNextSize(200, 100);
        ImGui::Begin("Function", NULL, m_WindowFlags);
        {
            ImGui::Text("Function 1");
            ImGui::SameLine(100);
            ImGui::Button("+");
        }
        ImGui::End();
    }

private:
    RobotFunction m_Function;
};