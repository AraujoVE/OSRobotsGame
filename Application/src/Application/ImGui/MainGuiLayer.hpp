#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"

using namespace DampEngine;
namespace Application
{

    class GameSave;
    class StatusWindow;
    class FunctionWindow;
    class MainGuiLayer final : public ImGuiLayer
    {
    public:
        MainGuiLayer(GameSave &gameSave);

    private:
        virtual void ImGuiDescription() override;

    private:
        GameSave &m_GameSave;
        StatusWindow* m_StatusWindow;
        FunctionWindow* m_FunctionWindows[RobotsManagement::FUNCTION_QTY];
    };
} // namespace Application