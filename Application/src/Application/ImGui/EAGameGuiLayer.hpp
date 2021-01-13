#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"

#include <memory>

namespace Application
{
    struct EAGameScriptData {
        
    };

    class GameSave;
    class GameRunner;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;
    class EAScript;
    class EAGameGuiLayer final : public DampEngine::ImGuiLayer
    {
    public:
        EAGameGuiLayer(const std::shared_ptr<GameSave> &gameSave);
        EAGameGuiLayer();
    private:
        virtual void ImGuiDescription() override;

    private:
        const static int SCRIPT_FUNCT_SIZE = 7;

        EAScript* m_scriptLoop;
        std::unique_ptr<GameRunner> m_GameRunner;
    };
} // namespace Application
