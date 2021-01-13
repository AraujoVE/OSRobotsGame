#include "Application/header/VillageStats.hpp"
#include "Application/header/ConstsMap.hpp"

#include "mypch.hpp"

namespace Application
{

    // ======================== GETS/SETS ========================
    //Each set must enter in the semaphore
    int VillageStats::getStat(int statType) const{
        return baseStats[statType];
    }

    int VillageStats::getPopulation() const
    {
        return population;
    }

    int VillageStats::getResources() const {
        return baseStats[4];
    }

    void VillageStats::setResources(int quantity) {
        baseStats[4] = quantity;
    }

    // ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
    VillageStats::VillageStats():
        ON_ATTACK_MULTIPLIER(ConstsMap::getValue("ON_ATTACK_MULTIPLIER")),
        POP_INCREASE_TAX(ConstsMap::getValue("POP_INCREASE_TAX")),
        POP_PER_CONSTRUCTION(ConstsMap::getValue("POP_PER_CONSTRUCTION")),
        INIT_POP_VALUE(ConstsMap::getValue("INIT_POP_VALUE")),
        INIT_STAT_VALUE(ConstsMap::getValue("INIT_STAT_VALUE")),
        ON_ATTACK_DECAY_TAX(ConstsMap::getValue("ON_ATTACK_DECAY_TAX")),
        NORMAL_DECAY_TAX(ConstsMap::getValue("NORMAL_DECAY_TAX")),
        ATTACK_FREQUENCY(ConstsMap::getValue("ATTACK_FREQUENCY")),
        INIT_RESOURCES_VALUE(ConstsMap::getValue("INIT_RESOURCES_VALUE")),
        TAX_REDUCT(ConstsMap::getValue("TAX_REDUCT")),
        DECAY_DELAY_MICRO(ConstsMap::getValue("DECAY_DELAY_MICRO")),
        MIN_LOSS{
            ConstsMap::getValue("MIN_LOSS_0"),
            ConstsMap::getValue("MIN_LOSS_1"),
            ConstsMap::getValue("MIN_LOSS_2"),
            ConstsMap::getValue("MIN_LOSS_3")
        },
        MAX_LOSS{
            ConstsMap::getValue("MAX_LOSS_0"),
            ConstsMap::getValue("MAX_LOSS_1"),
            ConstsMap::getValue("MAX_LOSS_2"),
            ConstsMap::getValue("MAX_LOSS_3")
        }
    {
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        initializeStats();
        initializeVSAvenues();
    
        
        pthread_create(&decayThread, NULL, runDecay, this);

        return;
    }

    void VillageStats::initializeStats()
    {
        for (int i = 0; i < BASE_STATS_NO - 1; i++) baseStats[i] = INIT_STAT_VALUE;
        
        baseStats[(int)RobotFunction::RESOURCE_GATHERING] = INIT_RESOURCES_VALUE;
        
        population = INIT_POP_VALUE;
        return;
    }

    void VillageStats::initializeVSAvenues() {
        for (int i = 0; i < BASE_STATS_NO; i++) {
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


    float VillageStats::calcRatio(int statType){
        return (float)baseStats[statType]/(float)population;
    }

    float VillageStats::calcReduction(float ratio,float decay){
        return (float)(rand()%101)/100.0 - ratio*decay;
    }

    float VillageStats::adjustStatsLimits(int statType,float reductionTax,float multiplier,bool changeAtAttack){
        if(reductionTax <= MIN_LOSS[statType]*multiplier){
            onAttack = changeAtAttack ? false : onAttack;
            reductionTax = MIN_LOSS[statType]*multiplier;
        }
        else{
            onAttack = changeAtAttack ? true : onAttack;
            if(reductionTax >= MAX_LOSS[statType]*multiplier){
                reductionTax = MAX_LOSS[statType]*multiplier;
            }
        }
        return reductionTax;
    }

    void VillageStats::setStat(int statType,float reductionTax){
        baseStats[statType] = (int)((float)baseStats[statType] * (1.0 - reductionTax));
    }


    //FIRST
    void VillageStats::decayDefenses(int it,int ratio,float &reduction){
        if(it == ATTACK_FREQUENCY) reduction = calcReduction(ratio,ON_ATTACK_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::PROTECTION,reduction,1,true);
    }

    //SECOND
    void VillageStats::decayFood(int it,int ratio,float &reduction){
        int inAttackVar = onAttack ? ON_ATTACK_MULTIPLIER : 1;

        if(onAttack) reduction = calcReduction(ratio,ON_ATTACK_DECAY_TAX);
        else reduction = calcReduction(ratio,NORMAL_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::PROTECTION,reduction,inAttackVar,false);

        statTax = 1.0 - reduction*TAX_REDUCT;

    }

    //THIRD
    void VillageStats::decayHealth(int it,int ratio,float &reduction){
        int inAttackVar = onAttack ? ON_ATTACK_MULTIPLIER : 1;

        if(onAttack) reduction = calcReduction(ratio,ON_ATTACK_DECAY_TAX*statTax);
        else reduction = calcReduction(ratio,NORMAL_DECAY_TAX*statTax);

        reduction = adjustStatsLimits((int)RobotFunction::MEDICINE,reduction,inAttackVar,false);

        statTax *= 1 - reduction*TAX_REDUCT;
    }

    //FOURTH
    void VillageStats::decayStructures(int it,int ratio,float &reduction){
        if(onAttack) reduction = calcReduction(ratio,NORMAL_DECAY_TAX);

        reduction = adjustStatsLimits((int)RobotFunction::CONSTRUCTION,reduction,1,false);

        maxPop = baseStats[(int)RobotFunction::CONSTRUCTION]*POP_PER_CONSTRUCTION;
    }

    //LAST
    void VillageStats::decayPopulation(){
        int popReduct = (int)((float)population * (1 - statTax));
        if((population - popReduct) > (int)(TAX_REDUCT * (float)population)){
            population -= popReduct;
        }
        else{
            population = (int)(TAX_REDUCT * (float)population);
        }
        population =  (int)((float)population * POP_INCREASE_TAX);
        if(population>maxPop) population = maxPop;
    }

    void VillageStats::decayStat(int it,int pos){
        avenueVS[pos]->down();

        float ratio = calcRatio(pos);
        float reduction = MIN_LOSS[pos];

        (this->*(decayStatsFuncts[pos]))(it,ratio,reduction);

        setStat(pos,reduction);

        avenueVS[pos]->up();
    }



    //The stats are decreased
    void VillageStats::decayStats()
    {
        int it = 0;
        while (!m_MarkedForDeletion) {
            avenueVS[POPULATION_INDEX]->down();
            
            onAttack = false;
            for(int i =0;i<BASE_STATS_NO - 1;i++) {
                decayStat(it,i);
            }

            decayPopulation();

            avenueVS[POPULATION_INDEX]->up();

            m_EventListener.On<EH_StatsDecayed>();
            
            if (ATTACK_FREQUENCY == 0) it = -1;
            else it = (it+1+ATTACK_FREQUENCY)%ATTACK_FREQUENCY;

            usleep(DECAY_DELAY_MICRO);
        }
    }

    void VillageStats::decayResources(int,int,float&) 
    {   
        DE_WARN("VillageStats::decayResources not implemented. Ignoring call!!!");
    }

    Avenue* VillageStats::getAvenue(int type) {
        return avenueVS[type];
    }

    void *runDecay (void *decayFuncObject) {
        VillageStats *village = (VillageStats*) decayFuncObject;
        village->decayStats();
        return NULL;
    }

    

    VillageStats::~VillageStats() {
        m_MarkedForDeletion = true;

        DE_DEBUG("IN: join @VillageStats::~VillageStats");
        pthread_join(decayThread, NULL);
        DE_DEBUG("OUT: join @VillageStats::~VillageStats");

        
        for (int i = 0; i < BASE_STATS_NO+1; i++)
        {
            avenueVS[i]->stopConsumer();
            delete avenueVS[i];
        }
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
                maxLoss = (int)MAX_LOSS * minMaxFact;
                minLoss = (int)MIN_LOSS * minMaxFact;
                // A value in range [100 - maxLoss,100 - minLoss]
                randVal = (100 - maxLoss) + rand() % (maxLoss - minLoss + 1);
                //set new absolut value 
                (this->*(setStatsFuncts[i]))((int)(baseStats[i] * ((float)randVal / 100.0)));

                avenueVS[i]->up();
            }
            */