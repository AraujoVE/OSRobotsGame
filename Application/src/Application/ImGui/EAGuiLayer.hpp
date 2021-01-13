#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"


namespace Application
{
    class GameSave;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;
    class EAScript;
    class EAGuiLayer final : public DampEngine::ImGuiLayer
    {
    public:
        EAGuiLayer(GameSave &gameSave);
    private:
        virtual void ImGuiDescription() override;

    private:
        void LostScreenDescription();
        const char *m_GameLostReason; 
        const static int SCRIPT_FUNCT_SIZE = 7;

        EAScript* m_scriptLoop;
        GameSave &m_GameSave;
    };
} // namespace Application
