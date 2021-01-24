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
        static const char* status = "Stopped";
        static GameRunner *lastGameRunner = m_EAGuiProps.MainGameRunner;
        bool settingsChanged = false;

        if (lastGameRunner != m_EAGuiProps.MainGameRunner) {
            settingsChanged = true;
            lastGameRunner = m_EAGuiProps.MainGameRunner;
        }

        ImGui::Begin("Settings");
        {
            settingsChanged |= ImGui::Checkbox("Show game", &m_EAGuiProps.ShowGame);
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

            ImGui::Checkbox("Pause", &m_EAGuiProps.Pause);
            ImGui::Text("Current Generation: %lu", (uint64_t) 0);
            ImGui::Text("Current Individual: %lu", (uint64_t) 0);
            ImGui::Text("Population: %lu", (uint64_t) 0);

            ImGui::Text("Total Robots: %lu", (uint64_t) 0);
            ImGui::Text("Resources: %lu", (uint64_t) 0);

        }
        ImGui::End();


        if (settingsChanged)
        {
            m_EventListener.OnAsync<EH_EAGuiPropsChanged>(m_EAGuiProps);
        }


        if (startPressed && !m_EAController->IsRunning()) {
            m_EAController->Start();
            
        }

        
    }

} // namespace Application
