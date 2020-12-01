#include "header/VillageStats.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

// ======================== GETS/SETS ========================
int VillageStats::getFood() {
    return food;
}

void VillageStats::setFood(int newFood) {
    if (newFood >= 0)
        food = newFood;
    return;
}

int VillageStats::getHealth() {
    return health;
}

void VillageStats::setHealth(int newHealth) {
    if (newHealth >= 0)
        health = newHealth;
    return;
}

int VillageStats::getStructures() {
    return structures;
}

void VillageStats::setStructures(int newStructures) {
    if (newStructures >= 0)
        structures = newStructures;
    return;
}

int VillageStats::getDefenses() {
    return defenses;
}

void VillageStats::setDefenses(int newDefenses) {
    if (newDefenses >= 0)
        defenses = newDefenses;
    return;
}

int VillageStats::getResources() {
    return resources;
}

void VillageStats::setResources(int newResources) {
    if (newResources >= 0)
        resources = newResources;
    return;
}

int VillageStats::getPopulation() {
    return population;
}

// ======================== CONSTRUCTOR / INITIALIZE VILLAGES STATS ========================
VillageStats::VillageStats() {
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    initializeStats();

    return;
}

void VillageStats::initializeStats() {
    food = 1+(maxStatValue/2);
    health = 1+(maxStatValue/2);
    structures = 1+(maxStatValue/2);
    defenses = 1+(maxStatValue/2);

    resources = initResourcesValue;
    population = initPopValue;
    
    return;
}

// ======================== CALCULATE NEW POPULATION ========================
void VillageStats::calcNewPop() {
    int threshold = maxStatValue/2;
    int maxPopVariation = 151;

    if (food > threshold)
        population += (food-threshold)*(std::rand() % maxPopVariation);
    else
        population -= (1+threshold-food)*(std::rand() % maxPopVariation);
    
    if (health > threshold)
        population += (health-threshold)*(std::rand() % maxPopVariation);
    else
        population -= (1+threshold-health)*(std::rand() % maxPopVariation);
    
    if (defenses > threshold)
        population += (defenses-threshold)*(std::rand() % maxPopVariation);
    else
        population -= (1+threshold-defenses)*(std::rand() % maxPopVariation);
    
    if (structures > threshold)
        population += (structures-threshold)*(std::rand() % maxPopVariation);
    else
        population -= (1+threshold-structures)*(std::rand() % maxPopVariation);
    
    std::cout << "New population: " << population << std::endl;
}

