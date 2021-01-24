#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "DampEngine/ImGui/ImGuiLayer.hpp"

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Game/GameRunner.hpp"

#include "EvoAlg/EAController.hpp"
#include "EvoAlg/Types.hpp"

#include <memory>

namespace Application
{
    class GameSave;
    class GameRunner;
    class StatusWindow;
    class FunctionWindow;
    class RobotCreationWindow;

    class EAGameGuiLayer final : public DampEngine::ImGuiLayer
    {
    public:
        EAGameGuiLayer();

        inline void SetOnSettingsChanged(EH_EAGuiPropsChanged* eventHandler) { m_EventListener.Register(eventHandler); }
    private:
        virtual void ImGuiDescription() override;


    private:
        //TODO: move to a better place
        const static int SCRIPT_FUNCT_SIZE = 7;

        EvoAlg::EAController *m_EAController;
        EventListener m_EventListener;


        EvoAlg::EAGuiProps m_EAGuiProps;
    };
} // namespace Application
