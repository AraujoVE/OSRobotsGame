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
// #include<cstring>
// #include<unistd.h>
// #include <thread>   /* std::this_thread::sleep_for */
// #include <chrono>   /* std::chrono::seconds */
// #include <GL/glut.h>
// #include <loguru.cpp> // logging lib -> https://emilk.github.io/loguru/index.html#loguru

#define NB_PARAMETERS 10 // number of parameters that will be adjusted
#define POPULATION_SIZE 20
#define MAX_MUTATION_RATE 0.04 // 4%

#define MAX_PARAM_VALUE 1 // max value a parameter can reach

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

// 3rd step: selection + mutation and crossover
void selection() { // tournament, elitism, roulette...
    /* 
    https://en.wikipedia.org/wiki/Selection_(genetic_algorithm)
    https://medium.com/datadriveninvestor/genetic-algorithms-selection-5634cfc45d78
    https://www.researchgate.net/publication/259461147_Selection_Methods_for_Genetic_Algorithms

    Stochastic Uniform (Fitness Proportionate) -> PROB THE BEST?
    */
    double normalizedFitness[POPULATION_SIZE];

    for (int i = 0; i < POPULATION_SIZE; i++) {
        normalizedFitness[i] = (fitness[i]-minFitness)/(maxFitness-minFitness);
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i == maxFitIndex) // keeping the best individual (no changes are made to it)
            continue;

        // Selection method???

        // Crossover
        // ...

        // Mutation
        for (int i = 0; i < POPULATION_SIZE; i++) { 
            for (int j = 0; j < NB_PARAMETERS; j++) {
                population[i][j] += ((double) ((double) (rand() % MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2) * (mutationRate));
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
    }
}