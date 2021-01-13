#include "Application/ImGui/EAGameGuiLayer.hpp"

#include "Application/Game/GameSave.hpp"
#include "Application/Game/GameRunner.hpp"
#include "Application/header/VillageStats.hpp"

namespace Application
{

    EAGameGuiLayer::EAGameGuiLayer()
        : m_GameRunner(new GameRunner())
    {
    }

    EAGameGuiLayer::EAGameGuiLayer(const std::shared_ptr<GameSave> &gameSave)
        : m_GameRunner(new GameRunner(gameSave))
    {
    }


    void EAGameGuiLayer::ImGuiDescription()
    {
        ImGui::Begin("Teste1");
        {
            ImGui::Text("Population: %d", m_GameRunner->GetSave().GetVillageStats()->getPopulation());
            bool s = ImGui::Button("Start decay");
            bool r = ImGui::Button("Restart");
            if (s) m_GameRunner->GetSave().GetVillageStats()->startStatsDecayment();
            if (r) m_GameRunner->ResetSave();
        }
        ImGui::End();
    }

} // namespace Application
