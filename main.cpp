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
[OK]
3. Fazer a função de cálculo do fitness (em evaluatePop) (fitness[i] = abs(terminoReal - terminoEsperado)/terminoEsperado)
[]
4. Fazer diferentes métodos de seleção, variação da taxa de mutação, uso de genocídio e predação etc.
[OK]
5. Fazer geração com os melhores dos outros AGs
Fazer POPULATION_SIZE+1 AGs. O último é um AG com os melhores de cada AG anterior
[ALTERADO]
-> Atualmente existem as finais e as semifinais. A final é composta de TOURNMENT_RATE semi-finais e cada semi-final é composta de TOURNMENT_RATE evolutionaryAlgorithms.
-> apos a ocorrencia de TOURNMENT_RATE EAS em uma semi-final, uma outra EA é feita, onde TOURNMENT_RATE dos individuos são os melhores individuos dessas EAs anteriores.
-> Na final é feito algo análogo à semi-final, só que ao invés dos melhores indivíduos virem das EAs, eles vem das semi-finais.

6. Fazer o AGs gerar o arquivo de configuração com os parâmetros do jogo (em initializePop)
[]

7. Passar pointeiro para fitness em normalizeFitness() e alterar o vetor fitness[] diretamente
[OK]
8. Fazer taxa de mutação crescer conforme nbNoImprovementGens aumenta (no momento, ela diminui)
[OK]
9. Checar se a variância é pequena para aplicar alteração mutação. Além disso, trocar em "if(nbNoImprovementGens > 200)", o 200 por um parâmetro (usar #define)
[NÂO NECESSARIO/A DISCUTIR]
10. Criar função de "genocídio" total
[OK] 
11. Salvar o melhor de cada AG em um vetor de "melhores globais" e trocar o while(true) da main por outra condição (o AG deve finalizar em algum momento - salvando seu melhor no vetor de "melhores globais")
[PARCIAL]
12. Em predationOfOne, remover if(generationIndex) e colocar if(noImprevementGens)
[OK]

*/
#include <stdlib.h>  /* srand, rand */
#include <iostream>
#include <fstream> // creating (.csv) files
#include <vector>
#include <cstring>
#include <functional>
#include <armadillo> // http://arma.sourceforge.net/docs.html
// #include <unistd.h>
// #include <thread>   /* std::this_thread::sleep_for */
// #include <chrono>   /* std::chrono::seconds */
// #include <GL/glut.h> /* simple GUI + keyboard usage */
// #include <loguru.cpp> // logging lib -> https://emilk.github.io/loguru/index.html#loguru

#define NB_PARAMETERS 10 // number of parameters that will be adjusted
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

arma::rowvec indvSemiFinals[TOURNMENT_RATE];
arma::rowvec indvFinals[TOURNMENT_RATE];

const int eventTriggerModuleBase[] = {1, 10, 5, 10, 2, 10};
int eventTriggerModule[6];

void (*eventTypes[])() = { increaseMutation, predationOfOne, partIncrease, oneRemainingPopReset, fullPopReset };

enum selectionMethods { TOURNAMENT, ELITISM, ROULETTE };
selectionMethods partsSelectionMethods[] = {
    TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,TOURNAMENT,
    ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM,ELITISM
};
int partPos = 0;
selectionMethods selectionMethod = partsSelectionMethods[partPos];

void (*selectionType[])() = { tournament, elitism, roulette };
arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> population; // arma::mat : armadillo matrix (Mat<double>)
// Each LINE (ROW) is the individual's parameters
double fitness[POPULATION_SIZE];
double maxFitness = 0, minFitness = 2000000000;
int maxFitIndex = -1, minFitIndex = -1;
double mutationRate = INITIAL_MUTATION;
int nbNoImprovementGens = 0; // number of generations in a row without any improvement (best individual remains the same)
int generationIndex = 0;
bool continueEA;

// Choose if population evaluation will be according to max fitness value or min fitness
enum evalTypes{ BY_MIN, BY_MAX };
evalTypes evalType = BY_MIN;
bool greater(double par1,double par2) { return par1 > par2; }
bool less(double par1,double par2) { return par1 < par2; }
bool (*evaluateType[])(double,double) = {less,greater};

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


void mutate(int indIndex) {
    int plusMinusSign = (rand() % 2 ? 1 : -1); // mutation will increase the param value or decrease it
    int mutateParamIndex = rand() % NB_PARAMETERS; // index of parameter that will be mutated

    population(indIndex, mutateParamIndex) += population(indIndex, mutateParamIndex) * mutationRate * plusMinusSign;

    return;
}


// Initialize .csv file to sabe data from the EA
// Creates the column's headers
// generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
void createCSV(std::string pathPos) {
    std::ofstream csvFileWriter;

    csvFileWriter.open("historyEA" + pathPos + ".csv");
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

    initializePop();
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

void evolutionaryAlgorithm(){
    nbNoImprovementGens = 0;
    createCSV("1");
    
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

void replaceByBests(arma::rowvec * bestsList){
    for(int i = 0; i < TOURNMENT_RATE; i++) 
        population.row(i) = bestsList[i];
}

void semiFinalsTournment(int semiFinalPos){
    for(int i = 0; i < TOURNMENT_RATE; i++){
        initializePop();
        evolutionaryAlgorithm();
        indvSemiFinals[i] = population.row(maxFitIndex); // saves the best individual from current EA
    }

    initializePop();
    replaceByBests(indvSemiFinals);
    evolutionaryAlgorithm(); // this EA will use the best individuals from each previous EA
    indvFinals[semiFinalPos] = population.row(maxFitIndex); // saves the best individual from current EA 
    return;
}

arma::rowvec finalTournment(){
    for(int i = 0; i < TOURNMENT_RATE; i++) 
        semiFinalsTournment(i);
    
    initializePop();
    replaceByBests(indvFinals); 
    evolutionaryAlgorithm(); // this EA will use the best individuals from each semifinal
    
    return population.row(maxFitIndex);
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    startEventTriggerList();

    arma::rowvec bestIndividual = finalTournment();

    system("clear");
}