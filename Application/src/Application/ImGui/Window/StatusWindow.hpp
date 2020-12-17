#pragma once

#include "Application/header/VillageStats.hpp"

#include "IGWindow.hpp"

#include "DampEngine/Core/Macros.hpp"

namespace Application
{

    void *spawnChange(void *abab);

    class StatusWindow final : IGWindow
    {
    public:
        StatusWindow(const VillageStats& villageStats)
            : m_VillageStatus(villageStats)
        {
            DE_INFO("{0}", (unsigned long)&villageStats);
        }

        void Render() override {
            SetNextPos();
            SetNextSize();

            unsigned population = m_VillageStatus.getPopulation();
            // unsigned stat1 = m_VillageStatus.getStat(1);


            bool issuedSpawn = false;
            ImGui::Begin("Status");
            {
                ImGui::Text("Population: %d", population);
                // ImGui::Text("Status 1: %d", stat1);


                issuedSpawn = ImGui::Button("Spawn producer");
            }
            ImGui::End();

            static bool spawned = false;
            if (issuedSpawn && !spawned) {
                // spawned = true;
                pthread_t t;
                pthread_create(&t, NULL, spawnChange, (void*)&m_VillageStatus);
            }
        }

    private:
        const VillageStats &m_VillageStatus;
    };

    

} // namespace Application