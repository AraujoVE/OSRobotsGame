#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"

using namespace DampEngine;
namespace Application
{

    class GameSave;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;
    class MainGuiLayer final : public ImGuiLayer
    {
    public:
        MainGuiLayer(GameSave &gameSave);

        inline FunctionWindow &GetFunctionWindow(RobotFunction function) const { return *m_FunctionWindows[(int)function]; }
    private:
        virtual void ImGuiDescription() override;

    private:
        void LostScreenDescription();
        bool m_GameLost;
        const char *m_GameLostReason; 

        GameSave &m_GameSave;
        StatusWindow* m_StatusWindow;
        FunctionWindow* m_FunctionWindows[FUNCTION_QTY];
        RobotCreationWindow* m_RobotCreationWindow;
    };
} // namespace Application