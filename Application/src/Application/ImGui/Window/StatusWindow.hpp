#pragma once

#include "Application/header/VillageStats.hpp"

#include "IGWindow.hpp"
namespace Application
{
    class StatusWindow final : IGWindow
    {
    public:
        StatusWindow(const VillageStats& villageStats)
            : m_VillageStatus(villageStats)
        {
        }

        void Render() override {
            SetNextPos();
            SetNextSize();

            unsigned population = m_VillageStatus.getPopulation();
            // unsigned stat1 = m_VillageStatus.getStat(1);


            ImGui::Begin("Status");
            {
                ImGui::Text("Population: %d", population);
                // ImGui::Text("Status 1: %d", stat1);
            }
            ImGui::End();
        }

    private:
        const VillageStats &m_VillageStatus;
    };

    

} // namespace Application