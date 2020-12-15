#include "header/VillageStats.hpp"
#include "header/RobotFunctions.hpp"
#include "header/RobotFunctions.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

namespace Application
{

    // ======================== GETS/SETS ========================
    //Each set must enter in the semaphore
    int VillageStats::getFood() const
    {
        return baseStats[(int)RobotFunction::HUNT];
    }

    void VillageStats::setFood(int newFood)
    {
        if (newFood >= 0)
            baseStats[(int)RobotFunction::HUNT] = newFood;
        return;
    }

    int VillageStats::getHealth() const
    {
        return baseStats[(int)RobotFunction::MEDICINE];
    }

    void VillageStats::setHealth(int newHealth)
    {
        if (newHealth >= 0)
            baseStats[(int)RobotFunction::MEDICINE] = newHealth;
        return;
    }

    int VillageStats::getStructures() const
    {
        return baseStats[(int)RobotFunction::CONSTRUCTION];
    }

    void VillageStats::setStructures(int newStructures)
    {
        if (newStructures >= 0)
            baseStats[(int)RobotFunction::CONSTRUCTION] = newStructures;
        return;
    }

    int VillageStats::getDefenses() const
    {
        return baseStats[(int)RobotFunction::PROTECTION];
    }

    void VillageStats::setDefenses(int newDefenses)
    {
        if (newDefenses >= 0)
            baseStats[(int)RobotFunction::PROTECTION] = newDefenses;
        return;
    }

    int VillageStats::getResources() const
    {
        return baseStats[(int)RobotFunction::RESOURCE_GATHERING];
    }

    void VillageStats::setResources(int newResources)
    {
        if (newResources >= 0)
            baseStats[(int)RobotFunction::RESOURCE_GATHERING] = newResources;
        return;
    }

    int VillageStats::getPopulation() const
    {
        return population;
    }

    // ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
    VillageStats::VillageStats()
    {
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        
        initializeStats();
        initializeAvenues();

        pthread_create(&decayThread, NULL, runDecay, this);

        return;
    }

    void VillageStats::initializeStats()
    {
        for (int i = 0; i < BASE_STATS_NO - 1; i++)
            baseStats[i] = 1 + (MAX_STAT_VALUE / 2);
        baseStats[(int)RobotFunction::RESOURCE_GATHERING] = INIT_RESOURCES_VALUE;
        population = INIT_POP_VALUE;
        return;
    }

    void VillageStats::initializeAvenues() {
        for (int i = 0; i < BASE_STATS_NO; i++) {
            avenue[i] = new Avenue(baseStats[i]);
            pthread_create(&consumers[i], NULL, runConsumer, avenue[i]);
        }

        avenue[POPULATION_INDEX] = new Avenue(population);
        pthread_create(&consumers[POPULATION_INDEX], NULL, runConsumer, avenue[POPULATION_INDEX]);
    }

    // ======================== ADD/REMOVE FOOD, MEDICINE ETC (STATS) OBTAINED FROM A COMPLETED TASK ========================

    //Increase in stats due to a task completion
    void VillageStats::changeStat(int type, int increase)
    {
        avenue[type]->producer(increase);
    }

    //The stats are decreased
    void VillageStats::decayStats ()
    {
        int randVal, maxLoss, minLoss;
        int currPopValue;
        int deltaPop;
        float minMaxFact;

        while (true) {
            currPopValue = avenue[POPULATION_INDEX]->getValue();
            for (int i = 0; i < BASE_STATS_NO - 1; i++) {
                avenue[i]->down();

                minMaxFact = (float)currPopValue / (float)baseStats[i];
                //The population per current stat ratio afects how much product will be lost
                maxLoss = (int)MAX_LOSS * minMaxFact;
                minLoss = (int)MIN_LOSS * minMaxFact;
                // A value in range [100 - maxLoss,100 - minLoss]
                randVal = (100 - maxLoss) + rand() % (maxLoss - minLoss + 1);
                //set new absolut value 
                (this->*(setStatsFuncts[i]))((int)(baseStats[i] * ((float)randVal / 100.0)));

                avenue[i]->up();
            }

            //IMPLEMENTA AQUI O CALCULO DE POPULACAO
            //deltaPop = ...

            // avenue[POPULATION_INDEX].producer(deltaPop);
            sleep(1);
        }
    }

    void *runDecay (void *decayFuncObject) {
        VillageStats *village = (VillageStats*) decayFuncObject;
        village->decayStats();
    }

    void *runConsumer (void *consumerObject) {
        Avenue *avenue = (Avenue*) consumerObject;
        avenue->consumer();
    }

} // namespace Application