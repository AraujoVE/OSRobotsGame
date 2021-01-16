#pragma once

#include "Application/header/VillageStats.hpp"

#include "Application/ImGui/IGWindow.hpp"
namespace Application::EAGameWindows
{
    class TimeWindow final : IGWindow
    {
    public:
        TimeWindow(const std::unique_ptr<VillageStats>& villageStats)
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

            ImGui::Begin("EAStatus");
            {
                ImGui::Text("Population: %d", population); ImGui::SameLine();
                ImGui::Text("Protection: %d", protection); ImGui::SameLine();
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