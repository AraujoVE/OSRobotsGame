#ifndef VILLAGE_STATS
#define VILLAGE_STATS

#include "RobotFunctions.hpp"
#include <ctime>

namespace Application
{
    class VillageStats
    {
    private:
        static const int BASE_STATS_NO = 5;
        const int MIN_LOSS = 5;
        const int MAX_LOSS = 10;
        const int MAX_STAT_VALUE = 5; // each village stat is between 0 and 5 (included)
        const int INIT_POP_VALUE = 1000;
        const int INIT_RESOURCES_VALUE = 100;
        int baseStats[BASE_STATS_NO];/*
            defenses;
            food;
            health;
            structures;
            resources;
        */
        void (VillageStats::*setStatsFuncts[BASE_STATS_NO])(int) = {
            &VillageStats::setFood,
            &VillageStats::setHealth,
            &VillageStats::setStructures,
            &VillageStats::setDefenses,
            &VillageStats::setResources
        };

        int (VillageStats::*getStatsFuncts[BASE_STATS_NO])() const = {
            &VillageStats::getFood,
            &VillageStats::getHealth,
            &VillageStats::getStructures,
            &VillageStats::getDefenses,
            &VillageStats::getResources
        };

        int population; // if population reaches zero, the game is over -> pop calculated based on other village stats
        //TODO: Aumentar e Diminuir tamanho da população
    public:
        VillageStats();

        int getFood() const;
        int getHealth() const;
        int getStructures() const;
        int getDefenses() const;
        int getResources() const;
        int getPopulation() const;

        void setFood(int);
        void setHealth(int);
        void setStructures(int);
        void setDefenses(int);
        void setResources(int);
        void setStat(int, int);

        void changeStat(int, int);
        void decayStats();

        void initializeStats();
        void calcNewPop();
        void addTaskResources(RobotFunction, time_t, int);
    };
} // namespace Application
#endif