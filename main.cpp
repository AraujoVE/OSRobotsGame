/*
- Etapas por geração:
1. Inicializar população
2. Avaliação
3. Seleção
4. Crossover 
5. Mutação
6. Rearranjo da população

// g++ -o main main.cpp -std=c++11  -O2 -larmadillo
[-lpthread -ldl -lglut -lGLU -lGL (-lm)] -> not used for now
// Warning flags: -Wall -Wextra -Werror
 

-> O que fazer agora?
1. Fazer a mutação só alterar um parâmetro de cada indivído ao invés de alterar todos os parâmetros
[OK]
2. Fazer "log" com os parâmetros, fitness etc. de cada geração (salvando, ao finalizar , a melhor versão dos parâmetros do jogo). No futuro, usar isso para plotar gráficos e tabelas
[]
3. Fazer a função de cálculo do fitness (em evaluatePop)
[]
4. Testar diferentes métodos de seleção, variação da taxa de mutação, uso de genocídio e predação etc.
[]
5. Fazer geração com os melhores dos outros AGs
[]
6. Fazer o AGs gerar o arquivo de configuração com os parâmetros do jogo (em initializePop)
[]

*/
#include <stdlib.h>  /* srand, rand */
#include <iostream>
#include <fstream> // creating (.csv) files
#include <vector>
#include <cstring>
#include <armadillo> // http://arma.sourceforge.net/docs.html
// #include <unistd.h>
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

arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> population; // arma::mat : armadillo matrix (Mat<double>)
// Each LINE (ROW) is the individual's parameters
double fitness[POPULATION_SIZE];
double maxFitness = 0, minFitness = 2000000000;
int maxFitIndex = -1;
double mutationRate = MAX_MUTATION_RATE;


// 1st step: initialize population
void initializePop() {
    std::cout << "INITIALIZING POPULATION\n";
    // for (int i = 0; i < POPULATION_SIZE; i++) {
    //     for (int j = 0; j < NB_PARAMETERS; j++) {
    //         population[i][j] = (double) (rand() % MAX_PARAM_VALUE); // number range = [0, MAX_PARAM_VALUE[
    //     }
    // }
    population.randu(); // initialize with values between 0 and 1
    population = population * MAX_PARAM_VALUE;

    population.print("Population matrix initialized:");

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
        // example of fitness calculation -> CHANGE!!!!
        fitness[i] = arma::sum(population.row(i));

        if (maxFitness < fitness[i]) { // searching for the  max fitnnes from new generation
            maxFitness = fitness[i];
            maxFitIndex = i;
        }
        else if (fitness[i] < minFitness)
            minFitness = fitness[i];

        // printf("fitness[%d] %lf\n", i, fitness[i]);
    }

    printf("MAX FITNESS: %lf - INDEX: %d\n", maxFitness, maxFitIndex);

    return;
}

void crossover(int i, arma::rowvec parent1, arma::rowvec parent2) {
    // for(int j = 0; j < NB_PARAMETERS; j++){
    //     population[i][j] = (parent1[j] + parent2[j])/2.0;        
    // }
    population.row(i) = (parent1 + parent2)/2.0;

    return;
}

void elitism() {
    std::cout << "ELITISM\n";
    arma::rowvec bestIndv = population.row(maxFitIndex);
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i == maxFitIndex) // keeping the best individual (no changes are made to it)
            continue;
        
        // crossover
        crossover(i, population.row(i), bestIndv);
    }

    return;
}

void tournament() {
    std::cout << "TOURNAMENT\n";
    arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> oldPopulation;
    int parentIndex[2]; 
    
    // copying last population (new one will be different)
    // for (int i = 0; i < POPULATION_SIZE; i++) {
    //     for (int j = 0; j < NB_PARAMETERS; j++) {
    //         oldPopulation[i][j] = population[i][j];
    //     }
    // }
    oldPopulation = population;

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
        crossover(i, oldPopulation.row(parentIndex[0]), oldPopulation.row(parentIndex[1]));
    }

    return;
}

void roulette() {
    std::cout << "ROULETTE\n";
    arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> oldPopulation;
    double standardizedFitness[POPULATION_SIZE];
    int parentIndex[2], rNb; 
    double probSum = 0.0, partialSum = 0.0;
    
    // copying last population (new one will be different)
    // for (int i = 0; i < POPULATION_SIZE; i++) {
    //     for (int j = 0; j < NB_PARAMETERS; j++) {
    //         oldPopulation[i][j] = population[i][j];
    //     }
    // }
    oldPopulation = population;

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
        crossover(i, oldPopulation.row(parentIndex[0]), oldPopulation.row(parentIndex[1]));
    }
}


// 3rd step: selection + mutation and crossover
void selection() { // tournament, elitism, roulette...
    std::cout << "SELECTION\n";
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

    // Isso aqui pode mudar, ou não, a mutação pode ocorrer em todos os parametros, ou não.
    // Isso depende de caso a caso, normalmente é feito a mutação em cada indivíduo, mas em 
    // apenas um parâmetro aleatório desse indivíduo. 
    // Todavia, em alguns casos é interessante fazer a mutação em todos os parâmtros como 
    // está sendo feito aqui. É importante testar.

    // Mutation

    // Mutating all params from individuals
    // arma::rowvec bestIndv = population.row(maxFitIndex);

    // arma::mat mutateMatrix(POPULATION_SIZE, NB_PARAMETERS, arma::fill::randu);
    // mutateMatrix = ((mutateMatrix * MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2.0) * mutationRate;

    // population = population + mutateMatrix;

    // population.row(maxFitIndex) = bestIndv;
    // population.transform( [](double x) { return ((x < 0 || x > MAX_PARAM_VALUE) ? abs(MAX_PARAM_VALUE-abs(x)) : x); } );
    
    // Mutating only one parameter from each individual
    for (int i = 0; i < POPULATION_SIZE; i++) { 
        if (i == maxFitIndex)
            continue; // don't mutate the best from last generation

        int mutateParamIndex = rand() % NB_PARAMETERS; // index of parameter that will be mutated
        population(i, mutateParamIndex) += (((double) (rand() % MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2.0) * (mutationRate));

        if (population(i, mutateParamIndex) < 0)
            population(i, mutateParamIndex) += MAX_PARAM_VALUE;
        else if (population(i, mutateParamIndex) > MAX_PARAM_VALUE)
            population(i, mutateParamIndex) -= MAX_PARAM_VALUE;
    }
    
    return;
}

// Initialize .csv file to sabe data from the EA
// Creates the column's headers
// generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
void createCSV() {
    std::ofstream csvFileWriter;

    csvFileWriter.open("historyEA.csv");
    if (!csvFileWriter.good()) {
        std::cout << "[!] Error occurred while trying to create historyEA.csv!\n";
        return;
    }
    
    csvFileWriter << "generation,";
    for (int i = 0; i < POPULATION_SIZE; i++)
        csvFileWriter << "paramsIndv" << i << ",fitnessIndv" << i << ",";
    csvFileWriter << "paramsBestIndv,fitnessBestIndv\n";
}

std::string formatParamsString(int indvIndex) {
    std::string paramsFormated = "[ ";
    for (int i = 0; i < NB_PARAMETERS; i++) {
        paramsFormated += std::to_string(population(indvIndex, i));
        paramsFormated += " ";
    }
    paramsFormated += "]";

    return paramsFormated;
}

// Saves information about a generation in a .csv file
// Information: generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
void saveGenerationData(int generation) {
    std::ofstream csvFileWriter;

    csvFileWriter.open("historyEA.csv", std::ios_base::app); // append instead of overwrite
    if (!csvFileWriter.good()) {
        std::cout << "[!] Error occurred while trying to open historyEA.csv!\n";
        return;
    }
    
    csvFileWriter << generation << ",";
    for (int i = 0; i < POPULATION_SIZE; i++) {
        csvFileWriter << formatParamsString(i) << "," << fitness[i] << ",";
    }
    csvFileWriter << formatParamsString(maxFitIndex) << "," << maxFitness << "\n";
}

int main(int argc, char * argv[]) {
    srand(time(NULL));

    system("clear");
    initializePop();
    createCSV();
    int generationIndex = 0;

    while (true) {
        std::cout << "\n==== Generation " << generationIndex << " ====\n";
        // population.print("Current population:");
        evaluatePop();
        selection();
        
        saveGenerationData(generationIndex);

        generationIndex++;
        scanf("%*c");
    }
}