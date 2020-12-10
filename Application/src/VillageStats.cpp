#include "header/VillageStats.hpp"
#include "header/RobotFunctions.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

// ======================== GETS/SETS ========================
//Each set must enter in the semaphore
int VillageStats::getFood() const {
    return food;
}

void VillageStats::setFood(int newFood) {
    if (newFood >= 0)
        food = newFood;
    return;
}

int VillageStats::getHealth() const {
    return health;
}

void VillageStats::setHealth(int newHealth) {
    if (newHealth >= 0)
        health = newHealth;
    return;
}

int VillageStats::getStructures() const {
    return structures;
}

void VillageStats::setStructures(int newStructures) {
    if (newStructures >= 0)
        structures = newStructures;
    return;
}

int VillageStats::getDefenses() const {
    return defenses;
}

void VillageStats::setDefenses(int newDefenses) {
    if (newDefenses >= 0)
        defenses = newDefenses;
    return;
}

int VillageStats::getResources() const {
    return resources;
}

void VillageStats::setResources(int newResources) {
    if (newResources >= 0)
        resources = newResources;
    return;
}

int VillageStats::getPopulation() const {
    return population;
}

// ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
VillageStats::VillageStats() {
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    initializeStats();

    return;
}

void VillageStats::initializeStats() {
    food = 1+(MAX_STAT_VALUE/2);
    health = 1+(MAX_STAT_VALUE/2);
    structures = 1+(MAX_STAT_VALUE/2);
    defenses = 1+(MAX_STAT_VALUE/2);

    resources = INIT_RESOURCES_VALUE;
    population = INIT_POP_VALUE;
    
    return;
}

// ======================== CALCULATE NEW POPULATION ========================
void VillageStats::calcNewPop() {
    int threshold = MAX_STAT_VALUE/2;
    int maxPopVariation = 151;
    
    auto calcPop = [=] (int par) {
        if (par > threshold)
            return (par-threshold)*(std::rand() % maxPopVariation);
        else
            return -((1+threshold-par)*(std::rand() % maxPopVariation));
    };
    
    population += calcPop(food);
    population += calcPop(health);
    population += calcPop(defenses);
    population += calcPop(structures);

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


// ======================== ADD FOOD, MEDICINE ETC (STATS) OBTAINED FROM A COMPLETED TASK ========================
void addTaskResources(RobotFunction taskType, time_t taskInitTime, int noRobots) {
    return;
}

void VillageStats::increaseStat(RobotFunction type,int increase,float multTax){
    if(type == RobotFunction::PROTECTION){
        this->setDefenses((int)((float)(defenses)*multTax) + increase);
    }
    if(type == RobotFunction::HUNT){
        this->setFood((int)((float)(food)*multTax) + increase);
    }
    if(type == RobotFunction::MEDICINE){
        this->setHealth((int)((float)(health)*multTax) + increase);
    }
    if(type == RobotFunction::CONSTRUCTION){
        this->setStructures((int)((float)(structures)*multTax) + increase);
    }
    if(type == RobotFunction::RESOURCE_GATHERING){
        this->setResources((int)((float)(resources)*multTax) + increase);
    }
};

void VillageStats::decreaseStats(){
    //!FIXME: [marcuscastelo] I've noticed division between ints in a multiplication factor calculation, please check if it's correct

    int randLoss = 0;
    int maxLoss,minLoss;

    static RobotFunction functions[] = { RobotFunction::PROTECTION, RobotFunction::HUNT, RobotFunction::MEDICINE, RobotFunction::CONSTRUCTION, RobotFunction::RESOURCE_GATHERING };
    float min_max_factors [] = { population/defenses, population/food, population/health, population/structures, population/resources };

    for (int i = 0; i < sizeof(functions)/sizeof(RobotFunction); i++)
    {
        maxLoss =  (int) MAX_LOSS * min_max_factors[i];
        minLoss =  (int) MIN_LOSS * min_max_factors[i];
        randLoss = (int) (100 - maxLoss) + rand()%(maxLoss - minLoss + 1);
        increaseStat((RobotFunction)i,0,rand()%randLoss);
    }
}

