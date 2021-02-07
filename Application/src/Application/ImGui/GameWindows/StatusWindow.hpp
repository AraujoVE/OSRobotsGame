#pragma once

#include "Application/Game/Ingame/VillageStats.hpp"

#include "Application/ImGui/IGWindow.hpp"
namespace Application::GameWindows
{
    class StatusWindow final : IGWindow
    {
    public:
        StatusWindow(const std::unique_ptr<VillageStats> &villageStats)
            : IGWindow(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar),
              m_VillageStatus(villageStats)
        {
            m_WindowProps.size = {1600, 40};
            m_WindowProps.pos = {0, 0};
        }

        void Render() override
        {
            SetNextPos();
            SetNextSize();

            uint64_t population = m_VillageStatus->getPopulation();
            uint64_t protection = m_VillageStatus->getStat(RobotFunction::PROTECTION);
            uint64_t hunt = m_VillageStatus->getStat(RobotFunction::HUNT);
            uint64_t medicine = m_VillageStatus->getStat(RobotFunction::MEDICINE);
            uint64_t construction = m_VillageStatus->getStat(RobotFunction::CONSTRUCTION);
            uint64_t resource_gathering = m_VillageStatus->getStat(RobotFunction::RESOURCE_GATHERING);

            const char *statusTitle = m_VillageStatus->isStatusDecaymentPaused() ? "Status (PAUSED)" : "Status";

            ImGui::Begin("GameStatus", NULL, m_WindowFlags);
            {
                ImGui::Text("%s",statusTitle);
                
                ImGui::Text("Population: %lu", population);
                ImGui::SameLine();
                ImGui::Text("Protection: %lu", protection);
                ImGui::SameLine();
                ImGui::Text("Hunt: %lu", hunt);
                ImGui::SameLine();
                ImGui::Text("Medicine: %lu", medicine);
                ImGui::SameLine();
                ImGui::Text("Construction: %lu", construction);
                ImGui::SameLine();
                ImGui::Text("Resources: %lu", resource_gathering);
            }
            ImGui::End();
        }

    private:
        const std::unique_ptr<VillageStats> &m_VillageStatus;
    };

} // namespace Application::GameWindows