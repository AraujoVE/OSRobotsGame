#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"

#include "Application/Events/EventHandler/DefaultHandlers.fwd.hpp"

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
        GameGuiLayer();
        void SetGameRunner(GameRunner*);
        
    private:
        void InitializeGameWindows();
        void DeinitializeGameWindows();

    private:
        virtual void ImGuiDescription() override;

        void NoGameAttachedGuiDescription();
        void MainGameGuiDescription();
        void GameLostGuiDescription();

    private:
        std::mutex m_GameRunnerMutex, m_GameWindowsMutex;
        
        
        const static int SCRIPT_FUNCT_SIZEE = 7;

        //It is not this class' responsibility to free game runner
        GameRunner *m_GameRunner;
        EH_GameStarted *m_GameStartedEventHandler;

        GameWindows::StatusWindow *m_StatusWindow;
        GameWindows::FunctionWindow *m_FunctionWindows[FUNCTION_QTY];  
        GameWindows::RobotCreationWindow *m_RobotCreationWindow;
    };
} // namespace Application