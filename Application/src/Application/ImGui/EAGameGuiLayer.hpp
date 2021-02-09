#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Game/GameRunner.hpp"

#include "EvoAlg/EAController.hpp"
#include "EvoAlg/Types.hpp"
#include "EvoAlg/Gui/EAGuiProps.hpp"

#include <memory>

namespace Application
{
    class GameSave;
    class GameRunner;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;
    class GameGuiLayer;

    class EAGameGuiLayer final : public DampEngine::ImGuiLayer
    {
    public:
        EAGameGuiLayer();
        
        void SetGameGuiLayer(GameGuiLayer*);
    private:
        virtual void ImGuiDescription() override;


    private:
        //TODO: move to a better place
        const static int SCRIPT_FUNCT_SIZE = 7;


        EvoAlg::EAController *m_EAController;
        EventListener m_EventListener;

        uint8_t elapsedGuiTicks = 0;
        //TODO: this should probably not be dependent on tick count
        const uint8_t ticksPerFlush = 30; //aprox. 500ms between flushs if 60fps

        Application::GameGuiLayer *m_GameGuiLayer = nullptr;
        EvoAlg::EAGuiProps m_EAGuiProps;
    };
} // namespace Application
