#pragma once

#include "Application/header/VillageStats.hpp"

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

            unsigned population = m_VillageStatus->getPopulation();
            unsigned protection = m_VillageStatus->getStat((int)RobotFunction::PROTECTION);
            unsigned hunt = m_VillageStatus->getStat((int)RobotFunction::HUNT);
            unsigned medicine = m_VillageStatus->getStat((int)RobotFunction::MEDICINE);
            unsigned construction = m_VillageStatus->getStat((int)RobotFunction::CONSTRUCTION);
            unsigned resource_gathering = m_VillageStatus->getStat((int)RobotFunction::RESOURCE_GATHERING);

            const char *statusTitle = m_VillageStatus->isStatusDecaymentPaused() ? "Status (PAUSED)" : "Status";

            ImGui::Begin("GameStatus", NULL, m_WindowFlags);
            {
                ImGui::Text("%s",statusTitle);
                
                ImGui::Text("Population: %d", population);
                ImGui::SameLine();
                ImGui::Text("Protection: %d", protection);
                ImGui::SameLine();
                ImGui::Text("Hunt: %d", hunt);
                ImGui::SameLine();
                ImGui::Text("Medicine: %d", medicine);
                ImGui::SameLine();
                ImGui::Text("Construction: %d", construction);
                ImGui::SameLine();
                ImGui::Text("Resources: %d", resource_gathering);
            }
            ImGui::End();
        }

    private:
        const std::unique_ptr<VillageStats> &m_VillageStatus;
    };

} // namespace Application::GameWindows