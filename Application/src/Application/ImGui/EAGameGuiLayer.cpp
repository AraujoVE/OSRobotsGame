#include "Application/ImGui/EAGameGuiLayer.hpp"

#include "Application/Game/GameSave.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/header/VillageStats.hpp"

#include "EAAlgorithm/EAScript.hpp"

#include "Application/header/ConstsMap.hpp"

using namespace EAAlgorithm;



namespace Application
{

    EAGameGuiLayer::EAGameGuiLayer(GameRunner &gameRunner)
        : m_GameRunner(gameRunner), m_EAController(gameRunner)
    {
    }

    void EAGameGuiLayer::ImGuiDescription()
    {
        //TODO: stop using static
        static const char* status = "Stopped";

        bool settingsChanged = false;
        ImGui::Begin("Settings");
        {
            settingsChanged |= ImGui::Checkbox("Show game", &m_Settings.ShowGame);
            settingsChanged |= ImGui::Checkbox("Pause game and EA", &m_Settings.PauseGame);

            settingsChanged |= ImGui::Checkbox("Manual mode", &m_Settings.ManualMode);
        }
        ImGui::End();

        if (settingsChanged)
        {
            m_EventListener.OnAsync<EH_EAGameSettingsChanged>(m_Settings);
        }

        bool startPressed = false;
        ImGui::Begin("EAStatus");
        {
            ImGui::Text("Evolutionary Algorithm (status: %s)", status);
            startPressed = ImGui::Button("Start EA");
        }
        ImGui::End();

        if (startPressed && !m_EAState.Started) {
            m_EAState.Started = true;
            m_EAController.StartEA();
        }

        
    }

} // namespace Application
