#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "EAAlgorithm/EAController.hpp"

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

    struct EAState
    {
        bool
            Started = false, 
            Running = false; 
    };


    class GameSave;
    class GameRunner;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;

    class EAGameGuiLayer final : public DampEngine::ImGuiLayer
    {
    public:
        EAGameGuiLayer();

        inline void SetOnSettingsChanged(EH_EAGameSettingsChanged* eventHandler) { m_EventListener.Register(eventHandler); }
    private:
        virtual void ImGuiDescription() override;


    private:
        //TODO: move to a better place
        const static int SCRIPT_FUNCT_SIZE = 7;

        //Not this class' responsibility to free
        GameRunner *m_MainGameRunner;


        EAAlgorithm::EAController *m_EAController;
        EventListener m_EventListener;


        EAGameSettings m_Settings;
        EAState m_EAState;
    };
} // namespace Application
