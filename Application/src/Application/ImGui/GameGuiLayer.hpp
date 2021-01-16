#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/header/RobotFunctions.hpp"

using namespace DampEngine;
namespace Application
{

    class GameSave;

    namespace GameWindows
    {
        class StatusWindow;
        class FunctionWindow;
        class RobotCreationWindow;
    } // namespace GameWindows

    class GameGuiLayer final : public ImGuiLayer
    {
    public:
        GameGuiLayer(GameRunner&);

    private:
        virtual void ImGuiDescription() override;

    private:
        void LostScreenDescription();
        const static int SCRIPT_FUNCT_SIZEE = 7;

        GameRunner &m_GameRunner;

        GameWindows::StatusWindow *m_StatusWindow;
        GameWindows::FunctionWindow *m_FunctionWindows[FUNCTION_QTY];
        GameWindows::RobotCreationWindow *m_RobotCreationWindow;
    };
} // namespace Application