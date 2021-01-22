#pragma once

#include "RobotFunctions.hpp"
#include "Application/Threads/Avenue.hpp"
#include <pthread.h>

#include "Application/Events/EventListener/EventListener.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Game/GameConsts.hpp"

#include "Application/Threads/ThreadLoop.hpp"

namespace Application
{
    class VillageStats
    {
    private:
        EventListener m_EventListener;
        GameConstsCache m_GameConstsCache;
    public:
        const static int BASE_STATS_NO = FUNCTION_QTY;
        const static int POPULATION_INDEX = BASE_STATS_NO;
    private:
        bool onAttack;
        double statTax;
        double maxPop;
        double population; // if population reaches zero, the game is over -> pop calculated based on other village stats

        double baseStats[BASE_STATS_NO];/*
            defenses;
            food;
            health;
            structures;
            resources;
        */

        Avenue<double> *avenueVS[BASE_STATS_NO+1];

        ThreadLoop m_DecayThreadLoop;

        void (VillageStats::*decayStatsFuncts[BASE_STATS_NO - 1])(int,float&) = {
            &VillageStats::decayFood,
            &VillageStats::decayHealth,
            &VillageStats::decayStructures,
            &VillageStats::decayDefenses,
        };

        unsigned int m_ElapsedTicks = 0;

        void initializeVSAvenues();
        bool updateGameConsts();

    public:
        VillageStats(GameConsts& gameConsts);
        VillageStats(const VillageStats&) = delete;
        VillageStats(VillageStats&&) = delete;
        ~VillageStats();

        float calcReduction(float,float);
        float calcRatio(int);
        float adjustStatsLimits(int,float,float,bool);

        void startStatsDecayment();
        void stopStatsDecayment();

        void setStatsDecaymentPaused(bool paused);
        inline bool isStatusDecaymentPaused() { return m_DecayThreadLoop.IsPaused(); }

        void decayStat(int);
        void setOnStatusDecayed(EH_StatsDecayed *eventHandler) { m_EventListener.Register(eventHandler); }

        void decayDefenses(int,float&);
        void decayFood(int,float&);
        void decayHealth(int,float&);
        void decayStructures(int,float&);
        void decayResources(int,float&);

        void decayPopulation();

        inline Avenue<double> *getAvenue(RobotFunction robotFunc) { return avenueVS[(uint8_t)robotFunc]; }
        uint64_t getStat(RobotFunction robotFunc) const;
        uint64_t getPopulation() const;
        uint64_t getResources() const;
        
        void setResources(uint64_t);

        void setStat(int,float);
        void changeStat(int type, int increase);
        void decayStats();

        void initializeStats();
        void addTaskResources(RobotFunction, time_t, int);

        unsigned int GetElapsedTimeTicks();
    };

    void *runDecay (void *decayFuncObject);
} // namespace Application