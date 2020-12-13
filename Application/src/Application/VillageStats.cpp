#include "header/VillageStats.hpp"
#include "header/RobotFunctions.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "header/RobotFunctions.hpp"

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

    // ======================== CALCULATE NEW POPULATION ========================
    void VillageStats::calcNewPop()
    {
        int threshold = MAX_STAT_VALUE / 2;
        int maxPopVariation = 151;

        auto calcPop = [=](int par) {
            if (par > threshold)
                return (par - threshold) * (std::rand() % maxPopVariation);
            else
                return -((1 + threshold - par) * (std::rand() % maxPopVariation));
        };

        for (int i = 0; i < BASE_STATS_NO - 1; i++)
        {
            population += calcPop(baseStats[i]);
        }

        //     if (food > threshold)
        //         population += (food-threshold)*(std::rand() % maxPopVariation);
        //     else
        //         population -= (1+threshold-food)*(std::rand() % maxPopVariation);

        //     if (health > threshold)
        //         population += (health-threshold)*(std::rand() % maxPopVariation);
        //     else
        //         population -= (1+threshold-health)*(std::rand() % maxPopVariation);

        //     if (defenses > threshold)
        //         population += (defenses-threshold)*(std::rand() % maxPopVariation);
        //     else
        //         population -= (1+threshold-defenses)*(std::rand() % maxPopVariation);

        //     if (structures > threshold)
        //         population += (structures-threshold)*(std::rand() % maxPopVariation);
        //     else
        //         population -= (1+threshold-structures)*(std::rand() % maxPopVariation);

        std::cout << "New population: " << population << std::endl;
    }

    // ======================== ADD/REMOVE FOOD, MEDICINE ETC (STATS) OBTAINED FROM A COMPLETED TASK ========================

    //Increase in stats due to a task completion
    void VillageStats::increaseStat(int type, int increase)
    {
        (this->*(setStatsFuncts[type]))(baseStats[type] + increase);
    }

    //The stats are decreased
    void VillageStats::decreaseStats()
    {
        int randVal, maxLoss, minLoss;
        float minMaxFact;

        for (int i = 0; i < BASE_STATS_NO - 1; i++)
        {
            minMaxFact = population / baseStats[i];
            //The population per current stat ratio afects how much product will be lost
            maxLoss = (int)MAX_LOSS * minMaxFact;
            minLoss = (int)MIN_LOSS * minMaxFact;
            // A value in range [100 - maxLoss,100 - minLoss]
            randVal = (100 - maxLoss) + rand() % (maxLoss - minLoss + 1);
            //set new value
            (this->*(setStatsFuncts[i]))((int)(baseStats[i] * ((float)randVal / 100.0)));
        }
    }

} // namespace Application