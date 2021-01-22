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
        : m_MainGameRunner(nullptr), m_EAController(new EAController())
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
            //TODO: attach gamerunner to gameGuiLayer in presentation mode
        }
        ImGui::End();

       
        static int a = 0;
        bool startPressed = false;
        ImGui::Begin("EAStatus");
        {
            ImGui::Text("Current Running Threads: 0");
            ImGui::SliderInt("Max threads", &a, 1, 10);


            ImGui::Text("Evolutionary Algorithm (status: %s)", status);
            startPressed = ImGui::Button("Start EA");

            settingsChanged |= ImGui::Checkbox("Pause", &m_Settings.PauseGame);
            ImGui::Text("Current Generation: %lu", (uint64_t) 0);
            ImGui::Text("Current Individual: %lu", (uint64_t) 0);
            ImGui::Text("Population: %lu", (uint64_t) 0);

            ImGui::Text("Total Robots: %lu", (uint64_t) 0);
            ImGui::Text("Resources: %lu", (uint64_t) 0);

        }
        ImGui::End();


        if (settingsChanged)
        {
            m_EventListener.OnAsync<EH_EAGameSettingsChanged>(m_Settings);
        }


        if (startPressed && !m_EAController->IsRunning()) {
            m_EAController->Start();
        }

        
    }

} // namespace Application
