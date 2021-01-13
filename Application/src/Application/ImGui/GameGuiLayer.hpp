#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/header/RobotFunctions.hpp"

using namespace DampEngine;
namespace Application
{

    class GameSave;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;

    class GameGuiLayer final : public ImGuiLayer
    {
    public:
        GameGuiLayer();
    private:
        virtual void ImGuiDescription() override;

    private:
        void LostScreenDescription();
        const static int SCRIPT_FUNCT_SIZEE = 7;

        GameRunner m_GameRunner;

        StatusWindow* m_StatusWindow;
        FunctionWindow* m_FunctionWindows[FUNCTION_QTY];
        RobotCreationWindow* m_RobotCreationWindow;
    };
} // namespace Application