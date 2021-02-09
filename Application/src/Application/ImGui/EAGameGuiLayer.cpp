#include "Application/ImGui/EAGameGuiLayer.hpp"

#include "Application/ImGui/GameGuiLayer.hpp"

#include "Application/Game/GameSave.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/Game/Ingame/VillageStats.hpp"

#include "EvoAlg/Script/ScriptRunner.hpp"

#include "Application/Game/GameConsts.hpp"

using namespace EvoAlg;
namespace Application
{

    EAGameGuiLayer::EAGameGuiLayer()
        : m_EAController(new EAController(m_EAGuiProps))
    {
    }

    void EAGameGuiLayer::SetGameGuiLayer(GameGuiLayer *guiLayer)
    {
        DE_ASSERT(m_GameGuiLayer == nullptr);
        m_GameGuiLayer = guiLayer;

        m_EAGuiProps.RegisterEAGameRunnerChanged(new EH_GameRunnerChanged(
            [guiLayer](ThreadID threadID, GameRunner *newGameRunner) {
                DE_ASSERT(threadID == 0, "multi-thread not yet supported!");
                guiLayer->SetGameRunner(newGameRunner);
                return false;
            }
        ));
    }

    void EAGameGuiLayer::ImGuiDescription()
    {
        bool settingsChanged = false;

        static int a = 0;
        ImGui::Begin("Settings");
        {
            // settingsChanged |= ImGui::Checkbox("Show game", &m_EAGuiProps.ShowGame);

            ImGui::Text("Current Running Threads: 0");
            ImGui::SliderInt("Max threads", &a, 1, 10);
        }
        ImGui::End();

        // uint32_t gameSpeed = 0, min = 1, max = 500e3;

        // if (m_EAGuiProps.MainGameRunner != nullptr)
        //     gameSpeed = m_EAGuiProps.MainGameRunner->GetGameConsts().GetTickDelay();
        // uint32_t receivedSpeed = gameSpeed;

        const EAStatus &eaStatus = m_EAController->GetStatus();
        static const char *const stageMessage[] = {
            "INACTIVE",
            "WAITING_GENERATION",
            "RUNNING_GENERATION",
            "FINISHED",
            "ABORTED"};

        unsigned int oldTickDelay = 0;
        unsigned int gameSpeed = oldTickDelay, min = 5, max = 1e6;

        bool startPressed = false, abortPressed = false;
        ImGui::Begin("EAStatus");
        {
            ImGui::Text("Evolutionary Algorithm (stage: %s)", stageMessage[(int)eaStatus.m_ExecutionInfo.Stage]);
            startPressed = ImGui::Button("Start EA");
            ImGui::SameLine();
            abortPressed = ImGui::Button("Abort EA");
            // ImGui::Checkbox("Pause EA and Game", &m_EAGuiProps.Pause);

            //TODO: *** currently not updating ThreadLoop ***
            ImGui::SliderScalar("Tick Delay", ImGuiDataType_U32, &gameSpeed, &min, &max, NULL, ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Current Generation: %lu", eaStatus.m_EvolutionInfo.CurrentGeneration);

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

        if (oldTickDelay != gameSpeed)
        {
            // m_EAGuiProps.MainGameRunner->GetGameConsts().SetTickDelay(gameSpeed);
        }

        if (settingsChanged)
        {
            m_EventListener.OnAsync<EH_GuiPropsChanged>(m_EAGuiProps);
        }

        if (startPressed && !m_EAController->IsRunning())
        {
            m_EAController->Start();
        }

        // if (gameSpeed != receivedSpeed)
        // {
        //     m_EAGuiProps.MainGameRunner->GetGameConsts().SetTickDelay(gameSpeed);
        // }
    }

} // namespace Application
