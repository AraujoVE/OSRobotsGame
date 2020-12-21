#ifndef VILLAGE_STATS
#define VILLAGE_STATS

#include "RobotFunctions.hpp"
#include "Avenue.hpp"

#include <pthread.h>

namespace Application
{
    class VillageStats
    {
    private:

        bool m_MarkedForDeletion = false;

    public:
        const static int BASE_STATS_NO = FUNCTION_QTY;
        const static int POPULATION_INDEX = BASE_STATS_NO;
        const int ON_ATTACK_MULTIPLIER;
        const float POP_INCREASE_TAX;
        const int POP_PER_CONSTRUCTION;
        const int INIT_POP_VALUE;
        const int INIT_STAT_VALUE;
        const float ON_ATTACK_DECAY_TAX;
        const float NORMAL_DECAY_TAX;
        const int ATTACK_FREQUENCY;
        const int INIT_RESOURCES_VALUE;
        const float TAX_REDUCT;
        const unsigned long DECAY_DELAY_MICRO;
    private:
        bool onAttack;
        float statTax;
        int maxPop;

        const float MIN_LOSS[BASE_STATS_NO - 1];
        const float MAX_LOSS[BASE_STATS_NO - 1];
        int baseStats[BASE_STATS_NO];/*
            defenses;
            food;
            health;
            structures;
            resources;
        */

        Avenue *avenueVS[BASE_STATS_NO+1];
        pthread_t decayThread;

        void (VillageStats::*decayStatsFuncts[BASE_STATS_NO])(int,int,float&) = {
            &VillageStats::decayFood,
            &VillageStats::decayHealth,
            &VillageStats::decayStructures,
            &VillageStats::decayDefenses,
            &VillageStats::decayResources
        };


        int population; // if population reaches zero, the game is over -> pop calculated based on other village stats
        //TODO: Aumentar e Diminuir tamanho da população

        void initializeVSAvenues();

    public:
        VillageStats();
        ~VillageStats();

        float calcReduction(float,float);
        float calcRatio(int);
        float adjustStatsLimits(int,float,float,bool);


        void decayStat(int,int);

        void decayDefenses(int,int,float&);
        void decayFood(int,int,float&);
        void decayHealth(int,int,float&);
        void decayStructures(int,int,float&);
        void decayResources(int,int,float&);

        void decayPopulation();

        Avenue *getAvenue(int type);

        int getStat(int) const;
        int getPopulation() const;
        int getResources() const;
        
        void setResources(int);

        void setStat(int,float);
        void changeStat(int type, int increase);
        void decayStats();

        void initializeStats();
        void addTaskResources(RobotFunction, time_t, int);
    };

    void *runDecay (void *decayFuncObject);
} // namespace Application
#endif