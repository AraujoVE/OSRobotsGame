#include "Application/ImGui/GameGuiLayer.hpp"

#include "mypch.hpp"

#include "Application/ImGui/GameWindows/FunctionWindow.hpp"
#include "Application/ImGui/GameWindows/StatusWindow.hpp"
#include "Application/ImGui/GameWindows/RobotCreationWindow.hpp"

#include "Application/Game/GameRunner.hpp"

using namespace Application::GameWindows;
namespace Application
{
    GameGuiLayer::GameGuiLayer() : m_GameRunner(nullptr), m_GameStartedEventHandler(nullptr)
    {
        m_GameStartedEventHandler = new EH_GameStarted([=](GameRunner &eventGR){
            DE_ASSERT(m_GameRunner != nullptr, "(GameGuiLayer) Receiving EH_GameStarted while no game is attached!! (did you forget to unregister the event?)");
            DE_ASSERT(m_GameRunner == &eventGR, "(GameGuiLayer) Receiving EH_GameStarted from an invalid GameRunner (did you forget to unregister the event?)");


            //Hacky way to update VillageStats and RobotsManagement references on the inner windows
            DeinitializeGameWindows();
            InitializeGameWindows();

            return false;
        });
    }

    void GameGuiLayer::SetGameRunner(GameRunner *newRunner)
    {
        if (m_GameRunner == newRunner) return;

        if (m_GameRunner != nullptr) {
            DeinitializeGameWindows();
            m_GameRunner->UnregisterOnGameStarted(m_GameStartedEventHandler);
        }

        m_GameRunner = newRunner;

        if (newRunner != nullptr) {
            InitializeGameWindows();
            m_GameRunner->RegisterOnGameStarted(m_GameStartedEventHandler);
        }
    }

    void GameGuiLayer::InitializeGameWindows()
    {
        DE_ASSERT(m_GameRunner != nullptr, "Trying to initialize game windows with no GameRunner attached");

        m_StatusWindow = new StatusWindow(m_GameRunner->GetSave().GetVillageStats());

        m_FunctionWindows[(int)RobotFunction::HUNT] = new FunctionWindow(m_GameRunner->GetSave().GetRobotsManagement(), RobotFunction::HUNT);
        m_FunctionWindows[(int)RobotFunction::CONSTRUCTION] = new FunctionWindow(m_GameRunner->GetSave().GetRobotsManagement(), RobotFunction::CONSTRUCTION);
        m_FunctionWindows[(int)RobotFunction::MEDICINE] = new FunctionWindow(m_GameRunner->GetSave().GetRobotsManagement(), RobotFunction::MEDICINE);
        m_FunctionWindows[(int)RobotFunction::PROTECTION] = new FunctionWindow(m_GameRunner->GetSave().GetRobotsManagement(), RobotFunction::PROTECTION);
        m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING] = new FunctionWindow(m_GameRunner->GetSave().GetRobotsManagement(), RobotFunction::RESOURCE_GATHERING);

        m_RobotCreationWindow = new RobotCreationWindow(m_GameRunner->GetSave().GetRobotsManagement());
        
        m_FunctionWindows[(int)RobotFunction::HUNT]->SetEventHandlers(m_GameRunner->GetSave().GetRobotsManagement());
        m_FunctionWindows[(int)RobotFunction::MEDICINE]->SetEventHandlers(m_GameRunner->GetSave().GetRobotsManagement());
        m_FunctionWindows[(int)RobotFunction::CONSTRUCTION]->SetEventHandlers(m_GameRunner->GetSave().GetRobotsManagement());
        m_FunctionWindows[(int)RobotFunction::PROTECTION]->SetEventHandlers(m_GameRunner->GetSave().GetRobotsManagement());
        m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING]->SetEventHandlers(m_GameRunner->GetSave().GetRobotsManagement());
    }

    void GameGuiLayer::DeinitializeGameWindows()
    {
        delete m_StatusWindow;

        delete m_FunctionWindows[(int)RobotFunction::HUNT];
        delete m_FunctionWindows[(int)RobotFunction::MEDICINE];
        delete m_FunctionWindows[(int)RobotFunction::CONSTRUCTION];
        delete m_FunctionWindows[(int)RobotFunction::PROTECTION];
        delete m_FunctionWindows[(int)RobotFunction::RESOURCE_GATHERING];

        delete m_RobotCreationWindow;
    }

    void GameGuiLayer::ImGuiDescription()
    {
        if (m_GameRunner == nullptr)
            NoGameAttachedGuiDescription();
        else if (m_GameRunner->IsGameLost())
            GameLostGuiDescription();
        else
            MainGameGuiDescription();
    }

    void GameGuiLayer::NoGameAttachedGuiDescription()
    {
        ImGui::Begin("NoGameAttached");
        {
            ImGui::Text("NO GAME IS ATTACHED TO THIS GUI");
        }
        ImGui::End();
    }

    void GameGuiLayer::MainGameGuiDescription()
    {
        DE_ASSERT(m_GameRunner != nullptr);

        ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None;

        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoResize;

        ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x, main_viewport->GetWorkPos().y));
        ImGui::SetNextWindowSize(ImVec2(main_viewport->GetWorkSize().x, 40));

        ImGui::Begin("Status", NULL, windowFlags);
        {
            ImGui::Text("Status");
        }
        ImGui::End();

        for (int i = 0; i < FUNCTION_QTY; i++)
        {
            m_FunctionWindows[i]->Render();
        }

        m_StatusWindow->Render();
        m_RobotCreationWindow->Render();
    }

    void GameGuiLayer::GameLostGuiDescription()
    {
        DE_ASSERT(m_GameRunner != nullptr);

        bool restart, quit;
        const static auto xSize = 300u, ySize = 125u;
        ImGui::SetNextWindowPos(
            {ImGui::GetMainViewport()->Pos.x + (ImGui::GetMainViewport()->Size.x - xSize) / 2,
             ImGui::GetMainViewport()->Pos.y + (ImGui::GetMainViewport()->Size.y - ySize) / 2});
        ImGui::SetNextWindowSize({xSize, ySize});
        ImGui::Begin("Game Over", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::Text("Game Over");
            ImGui::Text("%s", m_GameRunner->GetGameLostReason().c_str());

            ImGui::Text("Do you want to play again?");

            restart = ImGui::Button("Yes");
            ImGui::SameLine();
            quit = ImGui::Button("No");
        }
        ImGui::End();

        if (quit)
            DampEngine::Application::GetCurrent().Stop();
        else if (restart)
        {
            for (int i = 0; i < FUNCTION_QTY; i++)
                m_FunctionWindows[i]->ClearTaskWindows();

            m_GameRunner->Start();
        }
    }
} // namespace Application