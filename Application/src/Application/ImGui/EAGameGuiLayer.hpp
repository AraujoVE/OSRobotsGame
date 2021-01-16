#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"

#include <memory>

namespace Application
{
    struct EAGameSettings
    {
        bool
            ShowGame = false,
            PauseGame = false,
            ManualMode = false;
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

        inline void SetOnSettingsChanged(EH_EAGameSettingsChanged* eventHandler) { m_EventListener.Register(eventHandler); }
    private:
        virtual void ImGuiDescription() override;


    private:
        const static int SCRIPT_FUNCT_SIZE = 7;

        EAScript *m_scriptLoop;
        std::unique_ptr<GameRunner> m_GameRunner;
        EventListener m_EventListener;


        EAGameSettings m_Settings;
    };
} // namespace Application
