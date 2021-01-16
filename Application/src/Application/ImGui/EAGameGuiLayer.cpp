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

        bool changed = false;
        ImGui::Begin("Settings");
        {
            changed |= ImGui::Checkbox("Show game", &m_Settings.ShowGame);
            changed |= ImGui::Checkbox("Pause game and EA", &m_Settings.PauseGame);

            changed |= ImGui::Checkbox("Manual mode", &m_Settings.ManualMode);
        }
        ImGui::End();
        if (changed)
        {
            m_EventListener.OnAsync<EH_EAGameSettingsChanged>(m_Settings);
        }
    }

} // namespace Application
