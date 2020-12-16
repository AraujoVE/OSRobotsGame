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
    int VillageStats::getStat(int statType) const{
        return avenueVS[statType]->getValue();
    }

    int VillageStats::getPopulation() const
    {
        return avenueVS[POPULATION_INDEX]->getValue();
    }

    // ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
    VillageStats::VillageStats()
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
            pthread_create(&consumers[i], NULL, runConsumer, avenueVS[i]);
        }

        avenueVS[POPULATION_INDEX] = new Avenue(population);
        pthread_create(&consumers[POPULATION_INDEX], NULL, runConsumer, avenueVS[POPULATION_INDEX]);
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
        if((population - popReduct)>(int)(TAX_REDUCT * (float)population)){
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
    void VillageStats::decayStats ()
    {
        int it = 0;
        while (true) {
            avenueVS[POPULATION_INDEX]->down();

            for(int i =0;i<BASE_STATS_NO - 1;i++) decayStat(it,i);
            decayPopulation();

            avenueVS[POPULATION_INDEX]->up();

            it = (it+1)==ATTACK_FREQUENCY ? 0 : it+1;

            sleep(1);
        }
    }

    void *runDecay (void *decayFuncObject) {
        VillageStats *village = (VillageStats*) decayFuncObject;
        village->decayStats();
        return NULL;
    }

    void *runConsumer (void *consumerObject) {
        Avenue *avenue = (Avenue*) consumerObject;
        avenue->consumer();
        return NULL;
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