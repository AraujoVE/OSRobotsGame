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
        #define BTN(code, btnenum) { if( (code) && pressedButton == NONE) pressedButton = btnenum; } 
        enum PressedButton : uint8_t {
            NONE = 0, START_EA, ABORT_EA
        };

        PressedButton pressedButton = NONE;
        
        ImGui::Begin("Settings");
        {
            // settingsChanged |= ImGui::Checkbox("Show game", &m_EAGuiProps.ShowGame);

            ImGui::Text("Current Running Threads: 0");
            ImGui::SliderInt("Max threads", (int*)&m_EAGuiProps.m_Settings.SelectedMaxThreads, 1, 10);
        }
        ImGui::End();

        const EAStatus &eaStatus = m_EAController->GetStatus();
        static const char *const stageMessage[] = {
            "INACTIVE",
            "WAITING_GENERATION",
            "RUNNING_GENERATION",
            "FINISHED",
            "ABORTED"};

        ImGui::Begin("EAStatus");
        {
            ImGui::Text("Evolutionary Algorithm (stage: %s)", stageMessage[(int)eaStatus.m_ExecutionInfo.Stage]);
            BTN(ImGui::Button("Start EA"), START_EA);
            ImGui::SameLine();
            BTN(ImGui::Button("Abort EA"), ABORT_EA);
            ImGui::Checkbox("Pause EA and Game", &m_EAGuiProps.m_Settings.Paused);

            //TODO: *** currently not updating ThreadLoop ***
            const static uint32_t minTickDelay = 1, maxTickDelay = 10e6;
            ImGui::SliderScalar("Tick Delay", ImGuiDataType_U32, &m_EAGuiProps.m_Settings.SelectedTickDelay, &minTickDelay, &maxTickDelay, NULL, ImGuiSliderFlags_Logarithmic);

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

        if (pressedButton == START_EA && !m_EAController->IsRunning())
        {
            m_EAController->Start();
        }
    }

} // namespace Application
