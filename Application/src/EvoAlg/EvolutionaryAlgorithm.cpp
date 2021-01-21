#include <stdlib.h> /* srand, rand */
#include <iostream>
#include <fstream> // creating (.csv) files
#include <vector>
#include <cstring>
#include <string>
#include <functional>
#include <armadillo> // http://arma.sourceforge.net/docs.html
#include <thread>    /* std::this_thread::sleep_for */


#include "EvolutionaryAlgorithm.hpp"

#include "mypch.hpp"

#include "EAController.hpp"

// NOT USED??
// enum events{
//     BASE_RATE = 0,
//     MUTA_RATE,
//     PRED_RATE,
//     PART_RATE,
//     GENO_RATE,
//     EV_A_RATE,
// };

namespace EvoAlg
{
    //arma::rowvec semiFinalsAndFinals[SEMI_FINALS][TOURNMENT_RATE];
    //arma::rowvec indvFinals[TOURNMENT_RATE];
    void EvolutionaryAlgorithm::saveBestIndvParamsCSV()
    {
        std::fstream myFile;
        myFile.open("constValues.cfg", std::ios_base::app);

        myFile << attributesValues[0].name << " = " << population(bestFitIndex, 0);
        for (size_t i = 1; i < NB_PARAMETERS; i++)
            myFile << std::endl
                   << attributesValues[i].name << " = " << population(bestFitIndex, i);
        //printf("k%sk\n", parametersList[i]);
        myFile.close();
    }

    // 1st step: initialize population
    void EvolutionaryAlgorithm::initializePop(int tournmentType)
    {
        double minVal, maxVal;
        std::cout << "INITIALIZING POPULATION\n";
        // for (int i = 0; i < POPULATION_SIZE; i++) {
        //     for (int j = 0; j < NB_PARAMETERS; j++) {
        //         population[i][j] = (double) (rand() % MAX_PARAM_VALUE); // number range = [0, MAX_PARAM_VALUE[
        //     }
        // }
        population.randu(); // initialize with values between 0 and 1

        //TODO: find a better way to pass a non-variable to lambda
        // auto &_attributesValues = attributesValues;
        if (tournmentType == INITIALS) {
            int index = 0;
            population.each_col([=, &index](arma::vec& popCol) { popCol = attributesValues[index].min + popCol * (attributesValues[index].max - attributesValues[index].min); index++; } );
            // for(int j = 0; j < NB_PARAMETERS; j++)
            //     population.col(j) = attributesValues[j].min + population.col(j)*(attributesValues[j].max - attributesValues[j].min); // if a normal EA is taking place, just multiply the population by a givern value
        }
        else
        {
            int middle = TOURNMENT_RATE + (POPULATION_SIZE - TOURNMENT_RATE) / 2;
            fillInitialsWithBests(tournmentType);
            for (int j = 0; j < NB_PARAMETERS; j++)
            { // Min and max values are defined for each col, and converting the current col values to a number between this constraints.
                // this starts after the first TOURNMENT_RATE individuls, and the remaining is divided into two pars, one with biased rand values, and other without
                minVal = MIN_MULT * bestTournmentIndv[tournmentType].col(j).min();
                maxVal = MAX_MULT * bestTournmentIndv[tournmentType].col(j).max();

                for (int i = TOURNMENT_RATE; i < middle; i++)
                {
                    population(i, j) = minVal + (maxVal - minVal) * (population(i, j));
                }
            }
            for (int i = middle; i < POPULATION_SIZE; i++)
                for (int j = 0; j< NB_PARAMETERS; i++)
                    population(i,j) = attributesValues[j].min + population(i,j)*(attributesValues[j].max - attributesValues[j].min);
        }

        population.print("Population matrix initialized:");

        return;
    }

    // Function to normalize fitness (values between 0 and 1)
    void EvolutionaryAlgorithm::normalizeFitness(double *normalizedFitness)
    {
        if (normalizedFitness == NULL)
            return;

        for (int i = 0; i < POPULATION_SIZE; i++)
            normalizedFitness[i] = (fitness[i] - bestFitness) / (worstFitness - bestFitness);

        return;
    }

    // 2nd step: evaluate population (calculate fitness)
    void EvolutionaryAlgorithm::evaluatePop()
    {
        //Limit the values in each col
        // for(int j = 0; j < NB_PARAMETERS; j++){
        //     population.col(j) = arma::clamp(population.col(j),attributesValues[j].min,attributesValues[j].max);
        // }
        int index = 0;

        //TODO: find a better way to pass a non-variable to lambda
        auto &_attributesValues = attributesValues;
        population.each_col([&_attributesValues, &index](arma::vec& popCol) { popCol = arma::clamp(popCol, _attributesValues[index].min, _attributesValues[index].max); index++; } );

        std::cout << "EVALUATING POPULATION\n";

        nbNoImprovementGens++; // we begin considering there was no improvement in the generation


        std::vector<std::vector<double>> populationVec2(population.n_rows);
        for (size_t i = 0; i < population.n_rows; ++i)
            populationVec2[i] = (arma::conv_to<std::vector<double>>::from(population.row(i)));

        DE_TRACE("Sending population to EAController to execute it");
        auto gameplayResults = m_EAController.RunPopulationInGame(populationVec2);

        DE_TRACE("Calculating fitness of population after gameplay results received");
        calcFitness(gameplayResults);

        worstFitness = bestFitness = fitness[0];
        worstFitIndex = bestFitIndex = 0;

        for (int i = 1; i < POPULATION_SIZE; i++)
        {
            // example of fitness calculation -> CHANGE!!!!
            // fitness[i] = abs(terminoReal - terminoEsperado)/terminoEsperado

            DE_ASSERT(fitness[i] >= 0);
            if (fitness[i] < bestFitness)
            { // searching for the  max fitnnes from new generation
                bestFitness = fitness[i];
                bestFitIndex = i;
                nbNoImprovementGens = 0; // this generation shows improvement -> reset counter
            }
            else if (fitness[i] > worstFitness)
            {
                worstFitness = fitness[i];
                worstFitIndex = i;
            }

            // printf("fitness[%d] %lf\n", i, fitness[i]);
        }

        printf("BEST FITNESS: %lf - INDEX: %d\n", bestFitness, bestFitIndex);
        printf("WORST FITNESS: %lf - INDEX: %d\n", worstFitness, worstFitIndex);

        return;
    }

    void EvolutionaryAlgorithm::crossover(int indv, arma::rowvec parent1, arma::rowvec parent2)
    {
        // for(int j = 0; j < NB_PARAMETERS; j++){
        //     population[i][j] = (parent1[j] + parent2[j])/2.0;
        // }
        population.row(indv) = (parent1 + parent2) / 2.0;

        return;
    }

    void EvolutionaryAlgorithm::elitism()
    {
        std::cout << "ELITISM\n";
        arma::rowvec bestIndv = population.row(bestFitIndex);

        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            // crossover
            crossover(i, population.row(i), bestIndv);
        }

        return;
    }

    void EvolutionaryAlgorithm::tournament()
    {
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

        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            if (i == bestFitIndex)
                continue;

            // chossing parents for new individual
            for (int j = 0; j < 2; j++)
            {
                int indexIndA = rand() % POPULATION_SIZE; // indv 1 that will "fight" to be parent
                int indexIndB = rand() % POPULATION_SIZE; // indv 2 that will "fight" to be parent

                parentIndex[j] = (fitness[indexIndA] < fitness[indexIndB] ? indexIndA : indexIndB);
            }

            // crossover
            crossover(i, oldPopulation.row(parentIndex[0]), oldPopulation.row(parentIndex[1]));
        }

        return;
    }

    //TODO: swap priorities. Smaller fitness must be morre relevant
    void EvolutionaryAlgorithm::roulette()
    {
        std::cout << "ROULETTE\n";
        arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> oldPopulation;
        double standardizedFitness[POPULATION_SIZE];
        int parentIndex[2], rNb;
        double probSum = 0.0, partialSum = 0.0;

        (void) standardizedFitness[0];


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
            standardizedFitness[i] = fitness[i] / probSum;

        // Chosing new parents for each individual
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            if (i == bestFitIndex) // preserves best individual
                continue;

            for (int k = 0; k < 2; k++)
            {                                      // chosing 2 parents
                rNb = ((double)rand() / RAND_MAX); // rand between 0 and 1
                partialSum = 0.0;

                for (int j = 0; j < POPULATION_SIZE; j++)
                { // randomly chosing and individual according to its fitness (+fitness = +probabity)
                    partialSum += fitness[j];
                    if (partialSum >= rNb)
                    {
                        parentIndex[k] = j; // new parent at index j
                        break;
                    }
                }
            }

            // crossover
            crossover(i, oldPopulation.row(parentIndex[0]), oldPopulation.row(parentIndex[1]));
        }
    }

    void EvolutionaryAlgorithm::mutate(int indIndex)
    {
        int plusMinusSign = (rand() % 2 ? MUTATE_POSITIVE_MULT : MUTATE_NEGATIVE_MULT); // mutation will increase the param value or decrease it
        int mutateParamIndex = rand() % NB_PARAMETERS;                                  // index of parameter that will be mutated

        population(indIndex, mutateParamIndex) += population(indIndex, mutateParamIndex) * mutationRate * plusMinusSign;

        return;
    }

    // 3rd step: selection + mutation and crossover
    void EvolutionaryAlgorithm::selectionAndMutation()
    { // tournament, elitism, roulette...
        std::cout << "SELECTION\n";

        // Selection method (+ crossover)
        (this->*(selectionType[selectionMethod]))(); // void (*selectionType[])() => tournament(), elitism() or roulette()

        // Mutation

        // Mutating all params from individuals
        // arma::rowvec bestIndv = population.row(maxFitIndex);

        // arma::mat mutateMatrix(POPULATION_SIZE, NB_PARAMETERS, arma::fill::randu);
        // mutateMatrix = ((mutateMatrix * MAX_PARAM_VALUE) - MAX_PARAM_VALUE/2.0) * mutationRate;

        // population = population + mutateMatrix;

        // population.row(maxFitIndex) = bestIndv;
        // population.transform( [](double x) { return ((x < 0 || x > MAX_PARAM_VALUE) ? abs(MAX_PARAM_VALUE-abs(x)) : x); } );

        // Mutating only one parameter from each individual
        for (int i = 0; i < bestFitIndex; i++)
            mutate(i);
        // (don't mutate best one)
        for (int i = bestFitIndex + 1; i < POPULATION_SIZE; i++)
            mutate(i);

        return;
    }

    // Initialize .csv file to sabe data from the EA
    // Creates the column's headers
    // generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
    void EvolutionaryAlgorithm::createCSV(std::string pathPos)
    {
        std::ofstream csvFileWriter;

        csvFileWriter.open("historyEA-" + pathPos + ".csv");
        if (!csvFileWriter.good())
        {
            std::cout << "[!] Error occurred while trying to create historyEA" << pathPos << ".csv!\n";
            return;
        }

        csvFileWriter << "generation,";
        for (int i = 0; i < POPULATION_SIZE; i++)
            csvFileWriter << "paramsIndv" << i << ",fitnessIndv" << i << ",";
        csvFileWriter << "paramsBestIndv,fitnessBestIndv\n";

        return;
    }

    std::string EvolutionaryAlgorithm::formatParamsString(int indvIndex)
    {
        std::string paramsFormated = "[ ";
        for (int i = 0; i < NB_PARAMETERS; i++)
        {
            paramsFormated += std::to_string(population(indvIndex, i));
            paramsFormated += " ";
        }
        paramsFormated += "]";

        return paramsFormated;
    }

    // Saves information about a generation in a .csv file
    // Information: generation, paramsIndv1, fitnessIndv1 ..., paramsIndvN, fitnessIndvN, bestParams, bestFitness
    void EvolutionaryAlgorithm::saveGenerationData(int generation)
    {
        std::ofstream csvFileWriter;

        csvFileWriter.open("historyEA.csv", std::ios_base::app); // append instead of overwrite
        if (!csvFileWriter.good())
        {
            std::cout << "[!] Error occurred while trying to open historyEA.csv!\n";
            return;
        }

        csvFileWriter << generation << ",";
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            csvFileWriter << formatParamsString(i) << "," << fitness[i] << ",";
        }
        csvFileWriter << formatParamsString(bestFitIndex) << "," << bestFitness << "\n";
    }

    void EvolutionaryAlgorithm::increaseMutation()
    {
        if (!mutationRate)
        {
            mutationRate = INITIAL_MUTATION;
        }
        else
        {
            mutationRate *= MUTATION_INCREASE_RATIO;

            if (mutationRate > MAX_MUTATION_RATE) // mutation reached its max value
                mutationRate = MAX_MUTATION_RATE;
        }

        return;
    }

    // Function that will kill the worst individual each "APPLY_PREDATION_INTERVAL" number of generations
    void EvolutionaryAlgorithm::predationOfOne()
    {
        arma::rowvec newInd(NB_PARAMETERS, arma::fill::randu); // creating totally new individual
        // for(int j = 0; j < NB_PARAMETERS; j++){
        //     newInd(j) = attributesValues[j].min + newInd(j)*(attributesValues[j].max - attributesValues[j].min);
        // }
        int index = 0;
        //TODO: find a better way to pass a non-variable to lambda
        auto &_attributesValues = attributesValues;
        newInd.for_each([&_attributesValues, &index](double paramNewInd) { paramNewInd = _attributesValues[index].min + paramNewInd * (_attributesValues[index].max - _attributesValues[index].min); index++; } );

        population.row(worstFitIndex) = newInd;

        return;
    }

    // A sequence of selections will run, this method will indicate the selection method that will be used by the next batch of generations
    void EvolutionaryAlgorithm::partIncrease()
    {
        selectionMethod = partsSelectionMethods[++partPos];

        return;
    }

    // Function that will kill all individuals but the best to reset the population and generate new individuals (without biases)
    void EvolutionaryAlgorithm::oneRemainingPopReset()
    {
        std::cout << "APPLYING POPULATION RESET\n";

        arma::rowvec best = population.row(bestFitIndex);

        initializePop(INITIALS);
        population.row(0) = best;
        partIncrease();

        return;
    }

    // Ends the batch of generations from current EA
    void EvolutionaryAlgorithm::endEABatch()
    {
        std::cout << "END EA BATCH";

        continueBatch = false;

        return;
    }

    // Generic function that checks if certain event should happen (predation, population reset, mutation increase...)
    bool EvolutionaryAlgorithm::eventHappens(int eventType)
    {
        if (nbNoImprovementGens % eventTriggerModule[eventType] == 0)
            return true; // the event being verified should happen!
        return false;
    }

    void EvolutionaryAlgorithm::checkEvents()
    {
        if (nbNoImprovementGens == 0)
            return;

        for (int i = 4; i >= 0; i--)
        {
            if (eventHappens(i + 1))
            {
                (this->*(eventTypes[i]))(); // calls one of these functions: increaseMutation, predationOfOne, partIncrease, oneRemainingPopReset, fullPopReset
                return;
            }
        }
    }

    void EvolutionaryAlgorithm::startEventTriggerList()
    {
        for (int i = 1; i < 6; i++)
            eventTriggerModule[i] *= eventTriggerModule[i - 1];
    }

    void EvolutionaryAlgorithm::evoAlg(int createPopType,std::string csvStr)
    {
        initializePop(createPopType);
        nbNoImprovementGens = 0;
        createCSV(csvStr);
        continueBatch = true;
        partPos = 0; // defines selection method of current batch of generations

        generationIndex = 0;
        while (continueBatch)
        {
            std::cout << "\n==== Generation " << generationIndex << " ====\n";
            // population.print("Current population:");
            evaluatePop();
            selectionAndMutation();

            saveGenerationData(generationIndex);

            checkEvents(); // checks if mutation should increase, predation or population reset should occur etc.
            // if fullPopReset() is called, continueEA = false

            generationIndex++;
        }

        return;
    }

    void EvolutionaryAlgorithm::fillInitialsWithBests(int tournmentType)
    {
        for (int i = 0; i < TOURNMENT_RATE; i++)
            population.row(i) = bestTournmentIndv[tournmentType].row(i);
    }

    void EvolutionaryAlgorithm::semiFinalsTournment(int semiFinalPos)
    {
        for (int i = 0; i < TOURNMENT_RATE; i++)
        {
            evoAlg(INITIALS,"SF-" + std::to_string(semiFinalPos) + "_EA-" + std::to_string(i));
            bestTournmentIndv[SEMI_FINALS].row(i) = population.row(bestFitIndex); // saves the best individual from current EA
        }
        evoAlg(SEMI_FINALS,"SF-" + std::to_string(semiFinalPos));                               // this EA will use the best individuals from each previous EA
        bestTournmentIndv[FINALS].row(semiFinalPos) = population.row(bestFitIndex); // saves the best individual from current EA
        return;
    }

    void EvolutionaryAlgorithm::finalTournment()
    {
        for (int i = 0; i < TOURNMENT_RATE; i++)
            semiFinalsTournment(i);

        evoAlg(FINALS,"Main"); // this EA will use the best individuals from each semifinal

        bestIndividual = population.row(bestFitIndex);
        std::cout << "EVOLUTIONARY ALGORITHM FINISHED!" << std::endl;
        //eaFinished = true;
        return;
    }

    EvolutionaryAlgorithm::EvolutionaryAlgorithm(EAController &eaController) : m_EAController(eaController), eaFinished(false)
    {
        srand(time(NULL));
        startEventTriggerList();
    }

    static void *runScript(void *scriptFuncObject)
    {
        EvolutionaryAlgorithm *script = (EvolutionaryAlgorithm *)scriptFuncObject;
        script->finalTournment();
        return NULL;
    }

    void EvolutionaryAlgorithm::startAlgorithm()
    {
        pthread_create(&scriptThread, NULL, runScript, this);
    }

    EvolutionaryAlgorithm::~EvolutionaryAlgorithm()
    {
        pthread_join(scriptThread, NULL);
    }

    

    void EvolutionaryAlgorithm::calcFitness(const std::vector<GameplayResult> &gameplayResultsPop)
    {
        int pos = 0;
        for (auto &indivResults : gameplayResultsPop)
        {
            fitness[pos] = 0;
            for (auto &timeResult : indivResults)
            {
                fitness[pos] += std::abs(timeResult.TargetTime - timeResult.MeasuredTime) / timeResult.TargetTime;
            }
            pos++;
        }
        return;
    }

} // namespace EvoAlg
