#pragma once

#include "Application/header/VillageStats.hpp"

#include "IGWindow.hpp"
namespace Application
{
    class StatusWindow final : IGWindow
    {
    public:
        StatusWindow(const std::unique_ptr<VillageStats>& villageStats)
            : m_VillageStatus(villageStats)
        {
        }

        void Render() override {
            SetNextPos();
            SetNextSize();

            unsigned population = m_VillageStatus->getPopulation();
            unsigned protection = m_VillageStatus->getStat((int) RobotFunction::PROTECTION);
            unsigned hunt = m_VillageStatus->getStat((int) RobotFunction::HUNT);
            unsigned medicine = m_VillageStatus->getStat((int) RobotFunction::MEDICINE);
            unsigned construction = m_VillageStatus->getStat((int) RobotFunction::CONSTRUCTION);
            unsigned resource_gathering = m_VillageStatus->getStat((int) RobotFunction::RESOURCE_GATHERING);
            // unsigned stat1 = m_VillageStatus->getStat(1);

            ImGui::Begin("Status");
            {
                ImGui::Text("Population: %d", population); ImGui::SameLine();
                ImGui::Text("Protection: %d", protection);ImGui::SameLine();
                ImGui::Text("Hunt: %d", hunt); ImGui::SameLine();
                ImGui::Text("Medicine: %d", medicine);ImGui::SameLine();
                ImGui::Text("Construction: %d", construction); ImGui::SameLine();
                ImGui::Text("Resources: %d", resource_gathering);
            }
            ImGui::End();
        }

    private:
        const std::unique_ptr<VillageStats> &m_VillageStatus;
    };

    

} // namespace Application