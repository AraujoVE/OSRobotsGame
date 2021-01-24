#include "Application/ImGui/EAGameGuiLayer.hpp"

#include "Application/Game/GameSave.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/Game/Ingame/VillageStats.hpp"

#include "EvoAlg/ScriptRunner.hpp"

#include "Application/Game/GameConsts.hpp"

using namespace EvoAlg;
namespace Application
{

    EAGameGuiLayer::EAGameGuiLayer()
        : m_EAController(new EAController(m_EAGuiProps))
    {
    }

    void EAGameGuiLayer::ImGuiDescription()
    {
        //TODO: stop using static
        static const char *status = "Stopped";
        static GameRunner *lastGameRunner = m_EAGuiProps.MainGameRunner;
        bool settingsChanged = false;

        if (lastGameRunner != m_EAGuiProps.MainGameRunner)
        {
            settingsChanged = true;
            lastGameRunner = m_EAGuiProps.MainGameRunner;
        }
        static int a = 0;
        ImGui::Begin("Settings");
        {
            settingsChanged |= ImGui::Checkbox("Show game", &m_EAGuiProps.ShowGame);

            ImGui::Text("Current Running Threads: 0");
            ImGui::SliderInt("Max threads", &a, 1, 10);
        }
        ImGui::End();

        uint32_t gameSpeed = 0, min = 1, max = 500e3;

        if (m_EAGuiProps.MainGameRunner != nullptr)
            gameSpeed = m_EAGuiProps.MainGameRunner->GetGameConsts().GetTickDelay();
        uint32_t receivedSpeed = gameSpeed;

        bool startPressed = false, abortPressed = false;
        ImGui::Begin("EAStatus");
        {
            ImGui::Text("Evolutionary Algorithm (status: %s)", status);
            startPressed = ImGui::Button("Start EA");
            ImGui::SameLine();
            abortPressed = ImGui::Button("Abort EA");
            ImGui::Checkbox("Pause EA and Game", &m_EAGuiProps.Pause);

            if (gameSpeed > 0)
                ImGui::SliderScalar("Tick Delay", ImGuiDataType_U32, &gameSpeed, &min, &max, NULL, 1);

            ImGui::Text("Current Generation: %lu", (uint64_t)0);

            if (ImGui::TreeNode("Running Games"))
            {
                if (ImGui::TreeNode("Game 1"))
                {
                    ImGui::Text("Current Individual: %lu", (uint64_t)0);
                    ImGui::Text("Population: %lu", (uint64_t)0);

                    ImGui::Text("Total Robots: %lu", (uint64_t)0);
                    ImGui::Text("Resources: %lu", (uint64_t)0);
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();

        if (settingsChanged)
        {
            m_EventListener.OnAsync<EH_EAGuiPropsChanged>(m_EAGuiProps);
        }

        if (startPressed && !m_EAController->IsRunning())
        {
            m_EAController->Start();
        }

        if (gameSpeed != receivedSpeed)
        {
            m_EAGuiProps.MainGameRunner->GetGameConsts().SetTickDelay(gameSpeed);
        }
    }

} // namespace Application
