#include "Application/Game/Ingame/VillageStats.hpp"
#include "Application/Game/GameConsts.hpp"

#include "mypch.hpp"

namespace Application
{

    // ======================== GETS/SETS ========================
    //Each set must enter in the semaphore
    uint64_t VillageStats::getStat(RobotFunction robotFunc) const
    {
        return baseStats[(uint8_t)robotFunc];
    }

    uint64_t VillageStats::getPopulation() const
    {
        return population;
    }

    uint64_t VillageStats::getResources() const
    {
        return baseStats[4];
    }

    void VillageStats::setResources(uint64_t quantity)
    {
        baseStats[4] = quantity;
    }

    // ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
    VillageStats::VillageStats(GameConsts &gameConsts)
        : m_GameConstsCache(gameConsts), m_DecayThreadLoop("VillageStatsDecay")
    {
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        initializeStats();
        initializeVSAvenues();

        m_DecayThreadLoop.SetTickFunction(std::bind(&VillageStats::decayStats, this));
        m_DecayThreadLoop.SetAliveCheckFunction([this] {
            return this->getPopulation() > 0;
        });

        m_DecayThreadLoop.m_EventListener->Register(new EH_ThreadStarted([] {
            DE_TRACE("(VillageStats) m_DecayThreadLoop started successfully.");
            return false;
        }));

        m_DecayThreadLoop.m_EventListener->Register(new EH_ThreadEnded([](ThreadEndedReason::ThreadEndedReason_t reason) {
            DE_TRACE("(VillageStats) m_DecayThreadLoop ended. reason = {0}", reason);
            return false;
        }));
    }

    VillageStats::~VillageStats()
    {
        if (m_DecayThreadLoop.GetState() == ThreadLoop::State::RUNNING)
            m_DecayThreadLoop.Abandon();

        for (int i = 0; i < BASE_STATS_NO + 1; i++)
        {
            avenueVS[i]->stopConsumer();
            delete avenueVS[i];
        }
    }

    void VillageStats::initializeStats()
    {
        for (int i = 0; i < BASE_STATS_NO - 1; i++)
            baseStats[i] = m_GameConstsCache.INIT_STAT_VALUE;

        baseStats[(int)RobotFunction::RESOURCE_GATHERING] = m_GameConstsCache.INIT_RESOURCES_VALUE;

        population = m_GameConstsCache.INIT_POP_VALUE;
    }

    void VillageStats::initializeVSAvenues()
    {
        for (int i = 0; i < BASE_STATS_NO; i++)
        {
            avenueVS[i] = new Avenue(baseStats[i]);
            avenueVS[i]->startConsumer();
        }

        avenueVS[POPULATION_INDEX] = new Avenue(population);
        avenueVS[POPULATION_INDEX]->startConsumer();
    }

    // ======================== ADD/REMOVE FOOD, MEDICINE ETC (STATS) OBTAINED FROM A COMPLETED TASK ========================

    //Increase in stats due to a task completion
    void VillageStats::changeStat(int type, int increase)
    {
        avenueVS[type]->producer(increase);
    }

    float VillageStats::calcRatio(int statType)
    {
        return (double)baseStats[statType] / (double)population;
    }

    float VillageStats::calcReduction(float ratio, float decay)
    {
        return (float)(rand() % 101) / 100.0 - ratio * decay;
    }

    float VillageStats::adjustStatsLimits(int statType, float reductionTax, float multiplier, bool changeAtAttack)
    {
        if (reductionTax <= m_GameConstsCache.MIN_LOSS[statType] * multiplier)
        {
            onAttack = changeAtAttack ? false : onAttack;
            reductionTax = m_GameConstsCache.MIN_LOSS[statType] * multiplier;
        }
        else
        {
            onAttack = changeAtAttack ? true : onAttack;
            if (reductionTax >= m_GameConstsCache.MAX_LOSS[statType] * multiplier)
            {
                reductionTax = m_GameConstsCache.MAX_LOSS[statType] * multiplier;
            }
        }
        return reductionTax;
    }

    void VillageStats::setStat(int statType, float reductionTax)
    {
        baseStats[statType] = (uint64_t)((double)baseStats[statType] * (1.0 - reductionTax));
    }

    //FIRST
    void VillageStats::decayDefenses(int ratio, float &reduction)
    {
        if (m_ElapsedTicks % m_GameConstsCache.ATTACK_FREQUENCY == 0)
            reduction = calcReduction(ratio, m_GameConstsCache.ON_ATTACK_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::PROTECTION, reduction, 1, true);
    }

    //SECOND
    void VillageStats::decayFood(int ratio, float &reduction)
    {
        int inAttackVar = onAttack ? m_GameConstsCache.ON_ATTACK_MULTIPLIER : 1;

        if (onAttack)
            reduction = calcReduction(ratio, m_GameConstsCache.ON_ATTACK_DECAY_TAX);
        else
            reduction = calcReduction(ratio, m_GameConstsCache.NORMAL_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::PROTECTION, reduction, inAttackVar, false);

        statTax = 1.0 - reduction * m_GameConstsCache.TAX_REDUCT;
    }

    //THIRD
    void VillageStats::decayHealth(int ratio, float &reduction)
    {
        int inAttackVar = onAttack ? m_GameConstsCache.ON_ATTACK_MULTIPLIER : 1;

        if (onAttack)
            reduction = calcReduction(ratio, m_GameConstsCache.ON_ATTACK_DECAY_TAX * statTax);
        else
            reduction = calcReduction(ratio, m_GameConstsCache.NORMAL_DECAY_TAX * statTax);

        reduction = adjustStatsLimits((int)RobotFunction::MEDICINE, reduction, inAttackVar, false);

        statTax *= 1 - reduction * m_GameConstsCache.TAX_REDUCT;
    }

    //FOURTH
    void VillageStats::decayStructures(int ratio, float &reduction)
    {
        if (onAttack)
            reduction = calcReduction(ratio, m_GameConstsCache.NORMAL_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::CONSTRUCTION, reduction, 1, false);

        maxPop = baseStats[(int)RobotFunction::CONSTRUCTION] * m_GameConstsCache.POP_PER_CONSTRUCTION;
        if (maxPop < 0)
            maxPop = 1.7e308;
    }

    //LAST
    void VillageStats::decayPopulation()
    {
        double popReduct = ((double)population * (1 - statTax));
        if ((population - popReduct) > (uint64_t)(m_GameConstsCache.TAX_REDUCT * (double)population))
        {
            population -= popReduct;
        }
        else
        {
            population = m_GameConstsCache.TAX_REDUCT * (double)population;
        }
        population = (double)population * m_GameConstsCache.POP_INCREASE_TAX;
        if (population > maxPop)
            population = maxPop;
    }

    void VillageStats::startStatsDecayment()
    {
        DE_TRACE("VillageStats::startStatsDecayment()");
        m_DecayThreadLoop.Start(m_GameConstsCache.TICK_DELAY_MICRO);
    }

    void VillageStats::setStatsDecaymentPaused(bool paused)
    {
        DE_TRACE("VillageStats::setStatsDecaymentPaused({0})", paused);
        m_DecayThreadLoop.Pause(paused);
    }

    void VillageStats::decayStat(int pos)
    {
        avenueVS[pos]->down();

        float ratio = calcRatio(pos);
        float reduction = m_GameConstsCache.MIN_LOSS[pos];

        (this->*(decayStatsFuncts[pos]))(ratio, reduction);

        setStat(pos, reduction);

        avenueVS[pos]->up();
    }

    //The stats are decreased
    void VillageStats::decayStats()
    {
        avenueVS[POPULATION_INDEX]->down();

        onAttack = false;
        for (int i = 0; i < BASE_STATS_NO - 1; i++)
        {
            decayStat(i);
        }

        decayPopulation();

        avenueVS[POPULATION_INDEX]->up();

        //TODO: send more events
        if (population <= 0)
            m_EventListener.On<EH_StatsDecayed>();

        m_ElapsedTicks += 1;
        DE_TRACE("Tick = {0}", m_ElapsedTicks);
    }

    unsigned int VillageStats::GetElapsedTimeTicks()
    {
        return m_ElapsedTicks;
    }

} // namespace Application
  /*

        HUNT = 0,
        MEDICINE,
        CONSTRUCTION,
        PROTECTION,
        RESOURCE_GATHERING

c.i. = a Cada Iteração
TR = Taxa de Redução
RP = é Reduzido Por
RA = Reduz A
LA = Limita A
GO = Gera os
(fator atual -gera-> fator apontado)

CONSTRUCTION{
    TR(pequena),
    LA{
        POPULATION,
    },
    RP{
        Ataques(sempre -> médio/grande)    
    }
}
HUNT{
    TR(média)
    RA{
        MEDICINE(baixo -> médio),
        POPULATION(muito baixo -> baixo)        
    },
    RP{
        Ataques(sempre ->médio/grande)    
    }
}
MEDICINE{
    TR(alta)
    RA{
        POPULATION(baixa -> média)
    }
}
PROTECTION{
    TR(média)
    GO{
        Ataques
    }
}
*/

/*
            for (int i = 0; i < BASE_STATS_NO - 1; i++) {
                avenueVS[i]->down();

                minMaxFact = (float)currPopValue / (float)baseStats[i];
                //The population per current stat ratio afects how much product will be lost
                maxLoss = (int)m_GameConstsCache.MAX_LOSS * minMaxFact;
                minLoss = (int)m_GameConstsCache.MIN_LOSS * minMaxFact;
                // A value in range [100 - maxLoss,100 - minLoss]
                randVal = (100 - maxLoss) + rand() % (maxLoss - minLoss + 1);
                //set new absolut value 
                (this->*(setStatsFuncts[i]))((int)(baseStats[i] * ((float)randVal / 100.0)));

                avenueVS[i]->up();
            }
            */