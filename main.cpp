/*
- Etapas por geração:
1. Inicializar população
2. Avaliação
3. Seleção
4. Crossover 
5. Mutação
6. Rearranjo da população

// g++ -o main main.cpp -std=c++11 -lpthread -ldl 
[-lglut -lGLU -lGL (-lm)] -> not used
// Warning flags: -Wall -Wextra -Werror
 
*/
#include<stdlib.h>  /* srand, rand */
#include<iostream>
#include <vector>
// #include<cstring>
// #include<unistd.h>
// #include <thread>   /* std::this_thread::sleep_for */
// #include <chrono>   /* std::chrono::seconds */
// #include <GL/glut.h> /* simple GUI + keyboard usage */
// #include <loguru.cpp> // logging lib -> https://emilk.github.io/loguru/index.html#loguru

#define NB_PARAMETERS 10 // number of parameters that will be adjusted
#define POPULATION_SIZE 20
#define MAX_MUTATION_RATE 0.04 // 4%

#define MAX_PARAM_VALUE 1 // max value a parameter can reach

enum selectionMethods { TOURNAMENT, ELITISM, ROULETTE };
selectionMethods selectionMethod = ELITISM;

double population[POPULATION_SIZE][NB_PARAMETERS];
double fitness[NB_PARAMETERS];
double maxFitness = 0, minFitness = 2000000000;
int maxFitIndex = -1;
double mutationRate = MAX_MUTATION_RATE;


// 1st step: initialize population
void initializePop() {
    std::cout << "INITIALIZING POPULATION\n";
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NB_PARAMETERS; j++) {
            population[i][j] = (double) (rand() % MAX_PARAM_VALUE); // number range = [0, MAX_PARAM_VALUE[
        }
    }

    return;
}

double * normalizeFitness() {
    double normalizedFitness[POPULATION_SIZE];

    for (int i = 0; i < POPULATION_SIZE; i++) {
        normalizedFitness[i] = (fitness[i]-minFitness)/(maxFitness-minFitness);
    }

    return normalizedFitness;
}

// 2nd step: evaluate population (calculate fitness)
void evaluatePop() {
    std::cout << "EVALUATING POPULATION\n";

    // I don't know how the population will be evaluated yet...

    for (int i = 0; i < POPULATION_SIZE; i++) { 
        for (int j = 0; j < NB_PARAMETERS; j++) { // example of fitness calculation -> CHANGE!!!!
            fitness[i] += population[i][j];
        }

        if (maxFitness < fitness[i]) { // searching for the  max fitnnes from new generation
            maxFitness = fitness[i];
            maxFitIndex = i;
        }
        else if (fitness[i] < minFitness)
            minFitness = fitness[i];
    }
}

void elitism() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i == maxFitIndex) // keeping the best individual (no changes are made to it)
            continue;
        
        // crossover
        for (int j = 0; j < NB_PARAMETERS; j++) {
            population[i][j] = (population[i][j] + population[maxFitIndex][j])/2.0;
        }
    }

    return;
}

void tournament() {
    double oldPopulation[POPULATION_SIZE][NB_PARAMETERS];
    int parentIndex[2]; 
    
    // copying last population (new one will be different)
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NB_PARAMETERS; j++) {
            oldPopulation[i][j] = population[i][j];
        }
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i == maxFitIndex)
            continue;
        
        // chossing parents for new individual
        for (int j = 0; j < 2; j++) {
            int indexIndA = rand() % POPULATION_SIZE; // indv 1 that will "fight" to be parent
            int indexIndB = rand() % POPULATION_SIZE; // indv 2 that will "fight" to be parent

            parentIndex[j] = (fitness[indexIndA] > fitness[indexIndB]) ? indexIndA : indexIndB;
        }

        // crossover
        for (int j = 0; j < NB_PARAMETERS; j++) {
            population[i][j] = (oldPopulation[parentIndex[0]][j] + oldPopulation[parentIndex[1]][j])/2.0;
        }
    }

    return;
}

void roulette() {
    double oldPopulation[POPULATION_SIZE][NB_PARAMETERS];
    double standardizedFitness[POPULATION_SIZE];
    int parentIndex[2], rNb; 
    double probSum = 0.0, partialSum = 0.0;
    
    // copying last population (new one will be different)
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NB_PARAMETERS; j++) {
            oldPopulation[i][j] = population[i][j];
        }
    }

    // Standardize fitness (set probabilites that add up to 100%)
    for (int i = 0; i < POPULATION_SIZE; i++)
        probSum += fitness[i];
    for (int i = 0; i < POPULATION_SIZE; i++)
        standardizedFitness[i] = fitness[i]/probSum;
    
    // Chosing new parents for each individual
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i == maxFitIndex) // preserves best individual
            continue;
        
        for (int k = 0; k < 2; k++) { // chosing 2 parents
            rNb = ((double) rand() / RAND_MAX); // rand between 0 and 1
            partialSum = 0.0;

            for (int j = 0; j < POPULATION_SIZE; j++) { // randomly chosing and individual according to its fitness (+fitness = +probabity)
                partialSum += fitness[j];
                if (partialSum >= rNb) {
                    parentIndex[k] = j; // new parent at index j
                    break;
                }
            }
        }

        // crossover
        for (int j = 0; j < NB_PARAMETERS; j++) {
            population[i][j] = (oldPopulation[parentIndex[0]][j] + oldPopulation[parentIndex[1]][j])/2.0;
        }
    }
}

// 3rd step: selection + mutation and crossover
void selection() { // tournament, elitism, roulette...
    /* 
    https://en.wikipedia.org/wiki/Selection_(genetic_algorithm)
    https://medium.com/datadriveninvestor/genetic-algorithms-selection-5634cfc45d78
    https://www.researchgate.net/publication/259461147_Selection_Methods_for_Genetic_Algorithms

    Stochastic Uniform (Fitness Proportionate) -> PROB THE BEST?
    */

        // Selection method (+ crossover)
        switch (selectionMethod) {
            case TOURNAMENT:
                tournament();
                break;
            case ELITISM:
                elitism();
                break;
            case ROULETTE:
                roulette();
                break;
        }

        // Mutation
        for (int i = 0; i < POPULATION_SIZE; i++) { 
            if (i == maxFitIndex)
                continue; // don't mutate the best from last generation

            for (int j = 0; j < NB_PARAMETERS; j++) {
                population[i][j] += (((double) (rand() % MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2.0) * (mutationRate));
            }

            if (population[i][j] < 0)
                population[i][j] += MAX_PARAM_VALUE;
            else if (population[i][j] > MAX_PARAM_VALUE)
                population[i][j] -= MAX_PARAM_VALUE;
        }
    }
    



}

int main(int argc, char * argv[]) {
    srand(time(NULL));

    system("clear");
    initializePop();
    
    int generationIndex = 0;

    while (true) {
        std::cout << "\n==== Generation " << generationIndex << "====\n";
        evaluatePop();
        selection();
        generationIndex++;
    }
}