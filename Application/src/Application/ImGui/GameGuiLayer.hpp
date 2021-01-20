#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"

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
        
        const static int SCRIPT_FUNCT_SIZEE = 7;

        //Not this class responsibility to free game runner
        GameRunner *m_GameRunner;

        GameWindows::StatusWindow *m_StatusWindow;
        GameWindows::FunctionWindow *m_FunctionWindows[FUNCTION_QTY];
        GameWindows::RobotCreationWindow *m_RobotCreationWindow;
    };
} // namespace Application