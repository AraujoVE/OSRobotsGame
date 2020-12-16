#ifndef VILLAGE_STATS
#define VILLAGE_STATS

#include "RobotFunctions.hpp"
#include "Avenue.hpp"
#include <ctime>
#include <pthread.h>

namespace Application
{
    class VillageStats
    {
    private:
        static const int BASE_STATS_NO = 5;
        const int ON_ATTACK_MULTIPLIER = 2.0;
        const float POP_INCREASE_TAX = 1.1;
        const int POP_PER_CONSTRUCTION = 5;
        const int INIT_POP_VALUE = 1000;
        const int INIT_STAT_VALUE = 500;
        const float ON_ATTACK_DECAY_TAX = 0.6;
        const float NORMAL_DECAY_TAX = 0.9;
        const int ATTACK_FREQUENCY = 5;
        const int INIT_RESOURCES_VALUE = 100;
        const int POPULATION_INDEX = BASE_STATS_NO;
        const float TAX_REDUCT = 0.5;
        bool onAttack;
        float statTax;
        int maxPop;

        const int MIN_LOSS[BASE_STATS_NO - 1] = {
            0.05,
            0.05,
            0.05,
            0.025,
        };
        const int MAX_LOSS[BASE_STATS_NO - 1] = {
            0.5,
            0.25,
            0.25,
            0.125,
        };
        int baseStats[BASE_STATS_NO];/*
            defenses;
            food;
            health;
            structures;
            resources;
        */

        Avenue *avenueVS[BASE_STATS_NO+1];
        pthread_t decayThread;
        pthread_t consumers[BASE_STATS_NO+1];

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

        float calcReduction(float,float);
        float calcRatio(int);
        float adjustStatsLimits(int,float,float,bool);


        void decayStat(int,int);

        void decayFood(int,int,float&);
        void decayHealth(int,int,float&);
        void decayStructures(int,int,float&);
        void decayDefenses(int,int,float&);
        void decayResources(int,int,float&);

        void decayPopulation();



        int getStat(int) const;
        int getPopulation() const;

        void setStat(int,float);
        void changeStat(int, int);
        void decayStats();

        void initializeStats();
        void addTaskResources(RobotFunction, time_t, int);
    };

    void *runDecay (void *decayFuncObject);
    void *runConsumer (void *consumerObject);
} // namespace Application
#endif