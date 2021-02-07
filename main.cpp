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
 
-> Atualmente existem as finais e as semifinais entre AGs (AEs). A final é composta de TOURNMENT_RATE semi-finais e cada semi-final é composta de TOURNMENT_RATE evolutionaryAlgorithms.
-> apos a ocorrencia de TOURNMENT_RATE EAS em uma semi-final, uma outra EA é feita, onde TOURNMENT_RATE dos individuos são os melhores individuos dessas EAs anteriores.
-> Na final é feito algo análogo à semi-final, só que ao invés dos melhores indivíduos virem das EAs, eles vem das semi-finais.

-> O que fazer agora?
1. Fazer a função de cálculo do fitness (em evaluatePop) (fitness[i] = abs(terminoReal - terminoEsperado)/terminoEsperado)
[]
2. Fazer o AGs gerar o arquivo de configuração com os parâmetros do jogo (em initializePop)
[OK]
3. Checar se a variância é pequena para aplicar alteração mutação. Além disso, trocar em "if(nbNoImprovementGens > 200)", o 200 por um parâmetro (usar #define)
[NÂO NECESSARIO/A DISCUTIR]
4. Salvar o melhor de cada AG em um vetor de "melhores globais" e trocar o while(true) da main por outra condição (o AG deve finalizar em algum momento - salvando seu melhor no vetor de "melhores globais")
[PARCIAL]
5. Fazer nas semi-finais e finais initializePop com valores não puramente randomicos e sim com valores limitados entre o min e o max dos valores já descobertos para cada parametro
[OK]

*/
#include <stdlib.h>  /* srand, rand */
#include <iostream>
#include <fstream> // creating (.csv) files
#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include <armadillo> // http://arma.sourceforge.net/docs.html
// #include <unistd.h>
// #include <thread>   /* std::this_thread::sleep_for */
// #include <chrono>   /* std::chrono::seconds */
// #include <GL/glut.h> /* simple GUI + keyboard usage */
// #include <loguru.cpp> // logging lib -> https://emilk.github.io/loguru/index.html#loguru

const std::string parametersList[] = {
    "ON_ATTACK_MULTIPLIER",
    "POP_INCREASE_TAX",
    "POP_PER_CONSTRUCTION",
    "INIT_POP_VALUE",
    "INIT_STAT_VALUE",
    "ON_ATTACK_DECAY_TAX",
    "NORMAL_DECAY_TAX",
    "ATTACK_FREQUENCY",
    "INIT_RESOURCES_VALUE",
    "TAX_REDUCT",
    "DECAY_DELAY_MICRO",
    "MIN_LOSS_0",
    "MIN_LOSS_1",
    "MIN_LOSS_2",
    "MIN_LOSS_3",
    "MAX_LOSS_0",
    "MAX_LOSS_1",
    "MAX_LOSS_2",
    "MAX_LOSS_3",
    "TOT_ROBOTS_INI",
    "FREE_ROBOTS_INI",
    "PROD_COST_INI",
    "TIME_STEP",
    "INIT_TIME_STEP",
    "MAX_TIME_STEPS",
    "MIN_REWARD",
    "REWARD_RANGE",
    "FAILURE_TAX"
};

#define NB_PARAMETERS 28   // number of parameters that will be adjusted
#define POPULATION_SIZE 20

#define TOURNMENT_RATE 10

#define MAX_MUTATION_RATE 10
#define MUTATION_INCREASE_RATIO 1.1 // mutation increase 10% at a time
#define INITIAL_MUTATION 0.05

#define BEGIN_DECAY_THRESHOLD 50 // after how many generations without improvement the mutation starts to decay
#define APPLY_DECAY_MODULE 15 // decay will be applied when the generation index % APPLY_DECAY_MODULE = 0

#define APPLY_PREDATION_INTERVAL 10 // interval (in generations) to apply predation

#define MAX_PARAM_VALUE 1 // max value a parameter can reach



// NOT USED??
// enum events{
//     BASE_RATE = 0,
//     MUTA_RATE,
//     PRED_RATE,
//     PART_RATE,
//     GENO_RATE,
//     EV_A_RATE,
// };

enum tournments{ INITIALS,SEMI_FINALS, FINALS};

arma::mat::fixed<TOURNMENT_RATE,NB_PARAMETERS> indvSemiFinals; // arma::mat : armadillo matrix (Mat<double>)
arma::mat::fixed<TOURNMENT_RATE,NB_PARAMETERS> indvFinals; // arma::mat : armadillo matrix (Mat<double>)

arma::mat::fixed<TOURNMENT_RATE, NB_PARAMETERS> tournments[2] = {indvSemiFinals,indvFinals};

//arma::rowvec indvSemiFinals[TOURNMENT_RATE];
//arma::rowvec indvFinals[TOURNMENT_RATE];

const int eventTriggerModuleBase[] = {1, 10, 5, 10, 2, 10};
int eventTriggerModule[6];

enum selectionMethods { TOURNAMENT, ELITISM, ROULETTE };
selectionMethods partsSelectionMethods[] = {
    TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,
    ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM
};
int partPos = 0;
selectionMethods selectionMethod = partsSelectionMethods[partPos];

arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> population; // arma::mat : armadillo matrix (Mat<double>)
// Each LINE (ROW) is the individual's parameters
double fitness[POPULATION_SIZE];
double maxFitness = 0, minFitness = 2000000000;
int maxFitIndex = -1, minFitIndex = -1;
double mutationRate = INITIAL_MUTATION;
int nbNoImprovementGens = 0; // number of generations in a row without any improvement (best indiviNdual remains the same)
int generationIndex = 0;
bool continueEA;

// Choose if population evaluation will be according to max fitness value or min fitness
enum evalTypes{ BY_MIN, BY_MAX };
evalTypes evalType = BY_MIN;
bool greater(double par1,double par2) { return par1 > par2; }
bool less(double par1,double par2) { return par1 < par2; }
bool (*evaluateType[])(double,double) = {less,greater};

// 1st step: initialize population
void initializePop(int tournmentType) {
    double minVal,maxVal;
    std::cout << "INITIALIZING POPULATION\n";
    // for (int i = 0; i < POPULATION_SIZE; i++) {
    //     for (int j = 0; j < NB_PARAMETERS; j++) {
    //         population[i][j] = (double) (rand() % MAX_PARAM_VALUE); // number range = [0, MAX_PARAM_VALUE[
    //     }
    // }
    population.randu(); // initialize with values between 0 and 1
    if(!tournmentType) population *= MAX_PARAM_VALUE; // if a normal EA is taking place, just multiply the population by a givern value
    else{ // if not
        int middle = (POPULATION_SIZE - TOURNMENT_RATE)/2;
        for(int i = TOURNMENT_RATE + 1;i< middle;i++){ // Min and max values are defined for each col, and converting the current col values to a number between this constraints.
            // this starts after the first TOURNMENT_RATE individuls, and the remaining is divided into two pars, one with biased rand values, and other without
            minVal = 0.5 * tournments[tournmentType].col(i).min();
            maxVal = 1.5 * tournments[tournmentType].col(i).max();
            population.col(i) = minVal + (maxVal - minVal)*(population.col(i));
        }
        for(int i = middle + 1;i < POPULATION_SIZE;i++) 
            population.col(i) *= MAX_PARAM_VALUE;
    }

    std::fstream myFile;
    myFile.open("constValues.cfg", std::ios_base::app);
    
    myFile << parametersList[0] << " = " << population(0,0);
    for (size_t i = 1; i < NB_PARAMETERS; i++) myFile << std::endl << parametersList[i] << " = " << population(0,i);
    //printf("k%sk\n", parametersList[i]);
    myFile.close();

    population.print("Population matrix initialized:");

    return;
}

// Function to normalize fitness (values between 0 and 1)
void normalizeFitness(double * normalizedFitness) {
    if (normalizedFitness == NULL)
        return;
    
    for (int i = 0; i < POPULATION_SIZE; i++)
        normalizedFitness[i] = (fitness[i]-minFitness)/(maxFitness-minFitness);

    return;
}

// 2nd step: evaluate population (calculate fitness)
void evaluatePop() {
    std::cout << "EVALUATING POPULATION\n";

    nbNoImprovementGens++; // we begin considering there was no improvement in the generation

    for (int i = 0; i < POPULATION_SIZE; i++) { 
        // example of fitness calculation -> CHANGE!!!!
        // fitness[i] = abs(terminoReal - terminoEsperado)/terminoEsperado
        fitness[i] = arma::sum(population.row(i));

        if ((*evaluateType[evalType])(maxFitness,fitness[i])){ // searching for the  max fitnnes from new generation
            maxFitness = fitness[i];
            maxFitIndex = i;
            nbNoImprovementGens = 0; // this generation shows improvement -> reset counter
        }
        else if ((*evaluateType[evalType])(fitness[i],minFitness)){
            minFitness = fitness[i];
            minFitIndex = i;
        }

        // printf("fitness[%d] %lf\n", i, fitness[i]);
    }

    printf("MAX FITNESS: %lf - INDEX: %d\n", maxFitness, maxFitIndex);
    printf("MIN FITNESS: %lf - INDEX: %d\n", minFitness, minFitIndex);

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

void (*selectionType[])() = { tournament, elitism, roulette };

void mutate(int indIndex) {
    int plusMinusSign = (rand() % 2 ? 1 : -1); // mutation will increase the param value or decrease it
    int mutateParamIndex = rand() % NB_PARAMETERS; // index of parameter that will be mutated

    population(indIndex, mutateParamIndex) += population(indIndex, mutateParamIndex) * mutationRate * plusMinusSign;

    return;
}

// 3rd step: selection + mutation and crossover
void selectionAndMutation() { // tournament, elitism, roulette...
    std::cout << "SELECTION\n";

    // Selection method (+ crossover)
    (*selectionType[selectionMethod])(); // void (*selectionType[])() => tournament(), elitism() or roulette()

    // Mutation

    // Mutating all params from individuals
    // arma::rowvec bestIndv = population.row(maxFitIndex);

    // arma::mat mutateMatrix(POPULATION_SIZE, NB_PARAMETERS, arma::fill::randu);
    // mutateMatrix = ((mutateMatrix * MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2.0) * mutationRate;

    // population = population + mutateMatrix;

    // population.row(maxFitIndex) = bestIndv;
    // population.transform( [](double x) { return ((x < 0 || x > MAX_PARAM_VALUE) ? abs(MAX_PARAM_VALUE-abs(x)) : x); } );
    
    // Mutating only one parameter from each individual
    for(int i = 0; i < maxFitIndex; i++) mutate(i);
    // (don't mutate best one)
    for(int i = maxFitIndex+1; i < POPULATION_SIZE; i++) mutate(i);

    return;
}

// Initialize .csv file to sabe data from the EA
// Creates the column's headers
// generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
void createCSV(std::string pathPos) {
    std::ofstream csvFileWriter;

    csvFileWriter.open("historyEA-" + pathPos + ".csv");
    if (!csvFileWriter.good()) {
        std::cout << "[!] Error occurred while trying to create historyEA" << pathPos << ".csv!\n";
        return;
    }
    
    csvFileWriter << "generation,";
    for (int i = 0; i < POPULATION_SIZE; i++)
        csvFileWriter << "paramsIndv" << i << ",fitnessIndv" << i << ",";
    csvFileWriter << "paramsBestIndv,fitnessBestIndv\n";

    return;
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

void increaseMutation() {
    if(!mutationRate) {
        mutationRate = INITIAL_MUTATION;
    }
    else {
        mutationRate *= MUTATION_INCREASE_RATIO;

        if (mutationRate > MAX_MUTATION_RATE) // mutation reached its max value
            mutationRate = MAX_MUTATION_RATE;
    }

    return;
}

// Function that will kill the worst individual each "APPLY_PREDATION_INTERVAL" number of generations
void predationOfOne() {
    arma::rowvec newInd(NB_PARAMETERS, arma::fill::randu); // creating totally new individual
    newInd = newInd * MAX_PARAM_VALUE;
    
    population.row(minFitIndex) = newInd;

    return;
}

// A sequence of selections will run, this method will indicate the selection method that will be used by the next batch of generations
void partIncrease(){
    selectionMethod = partsSelectionMethods[++partPos];

    return;
}

// Function that will kill all individuals but the best to reset the population and generate new individuals (without biases)
void oneRemainingPopReset() {
    std::cout << "APPLYING POPULATION RESET\n";

    arma::rowvec best = population.row(maxFitIndex);

    initializePop(INITIALS);
    population.row(0) = best;
    partIncrease();

    return;
}


// Function that kills all individuals and creates a new population
// Ends the batch of generations from current EA
void fullPopReset() {
    std::cout << "HARD POPULATION RESET";

    continueEA = false;

    return;
}

// Generic function that checks if certain event should happen (predation, population reset, mutation increase...)
bool eventHappens(int eventType) {
    if(nbNoImprovementGens % eventTriggerModule[eventType] == 0) 
        return true; // the event being verified should happen!
    return false;
}

void (*eventTypes[])() = { increaseMutation, predationOfOne, partIncrease, oneRemainingPopReset, fullPopReset };


void checkEvents() {
    if(nbNoImprovementGens == 0) 
        return;
    
    for(int i = 1; i < 6; i++){
        if(eventHappens(i)) {
            (*eventTypes[i-1])(); // calls one of these functions: increaseMutation, predationOfOne, partIncrease, oneRemainingPopReset, fullPopReset
            return;
        }
    }
}

void startEventTriggerList(){
    eventTriggerModule[0] = eventTriggerModuleBase[0];
    for(int i = 1; i < 6; i++)
        eventTriggerModule[i] = eventTriggerModule[i-1] * eventTriggerModuleBase[i];
}

void evolutionaryAlgorithm(std::string csvStr){
    nbNoImprovementGens = 0;
    createCSV(csvStr);
    
    partPos = 0; // defines selection method of current batch of generations

    continueEA = true;
    generationIndex = 0;
    while (continueEA) {
        std::cout << "\n==== Generation " << generationIndex << " ====\n";
        // population.print("Current population:");
        evaluatePop();
        selectionAndMutation();
        
        saveGenerationData(generationIndex);

        checkEvents(); // checks if mutation should increase, predation or population reset should occur etc.
        // if fullPopReset() is called, continueEA = false

        generationIndex++;
        scanf("%*c");
    }

    return;
}

void replaceByBests(int tournmentType){
    for(int i = 0; i < TOURNMENT_RATE; i++) 
        population.row(i) = tournments[tournmentType - 1].row(i);
}

void semiFinalsTournment(int semiFinalPos){
    for(int i = 0; i < TOURNMENT_RATE; i++){
        initializePop(INITIALS);
        evolutionaryAlgorithm("SF-" + std::to_string(semiFinalPos) + "_EA-" + std::to_string(i));
        indvSemiFinals.row(i) = population.row(maxFitIndex); // saves the best individual from current EA
    }

    initializePop(SEMI_FINALS);
    replaceByBests(SEMI_FINALS);
    evolutionaryAlgorithm("SF-" + std::to_string(semiFinalPos)); // this EA will use the best individuals from each previous EA
    indvFinals.row(semiFinalPos) = population.row(maxFitIndex); // saves the best individual from current EA 
    return;
}

arma::rowvec finalTournment(){
    for(int i = 0; i < TOURNMENT_RATE; i++) 
        semiFinalsTournment(i);
    
    initializePop(FINALS);
    replaceByBests(FINALS); 
    evolutionaryAlgorithm("Main"); // this EA will use the best individuals from each semifinal
    
    return population.row(maxFitIndex);
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    startEventTriggerList();

    arma::rowvec bestIndividual = finalTournment();

    system("clear");
}