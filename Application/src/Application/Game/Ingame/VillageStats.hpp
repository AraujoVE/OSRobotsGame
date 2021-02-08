#pragma once

#include "RobotFunctions.hpp"
#include "Application/Threads/Avenue.hpp"

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

        double baseStats[BASE_STATS_NO]; /*
            defenses;
            food;
            health;
            structures;
            resources;
        */

        Avenue<double> *avenueVS[BASE_STATS_NO + 1];

        ThreadLoop *m_DecayThreadLoop;

        void (VillageStats::*decayStatsFuncts[BASE_STATS_NO - 1])(int, float &) = {
            &VillageStats::decayFood,
            &VillageStats::decayHealth,
            &VillageStats::decayStructures,
            &VillageStats::decayDefenses,
        };

        unsigned int m_ElapsedTicks = 0;

        void initializeVSAvenues();
        bool updateGameConsts();

    private:
        void initializeStats();

        float calcReduction(float, float);
        float calcRatio(int);
        float adjustStatsLimits(int, float, float, bool);

        void decayStat(int);

        void decayDefenses(int, float &);
        void decayFood(int, float &);
        void decayHealth(int, float &);
        void decayStructures(int, float &);
        void decayResources(int, float &);

        void decayPopulation();

    public:
        VillageStats(GameConsts &gameConsts);
        ~VillageStats();

        void onGameStarted();
        void onGameEnded();
        inline ThreadLoop &getThreadLoop() { return *m_DecayThreadLoop; }

        void setStatsDecaymentPaused(bool paused);
        inline bool isStatusDecaymentPaused() const { return m_DecayThreadLoop->IsPaused(); }

        inline Avenue<double> *getAvenue(RobotFunction robotFunc) { return avenueVS[(uint8_t)robotFunc]; }
        uint64_t getStat(RobotFunction robotFunc) const;
        uint64_t getPopulation() const;
        uint64_t getResources() const;

        void changeStat(int type, int increase);
        void setStat(RobotFunction, float reductionTax);
        void setResources(uint64_t);

        void DecayStats();

        inline void ClearEvents() { m_EventListener.Clear(); }
        
        inline void RegisterOnStatsDecaymentStarted(EH_DecaymentStarted *eventHandler) { m_EventListener.Register(eventHandler); }
        inline void RegisterOnStatsDecaymentStopped(EH_DecaymentStopped *eventHandler) { m_EventListener.Register(eventHandler); }

        inline void UnregisterOnStatsDecaymentStopped(EH_DecaymentStopped *eventHandler) { m_EventListener.Unregister(eventHandler); }
        inline void UnregisterOnStatsDecaymentStarted(EH_DecaymentStarted *eventHandler) { m_EventListener.Unregister(eventHandler); }
        
        unsigned int GetElapsedTimeTicks();
    };
} // namespace Application