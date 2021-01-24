#pragma once

//Types such as GeneVec, Individual, GameplayResults, etc.
#include "EvoAlg/Types.hpp"

#include <string>
#include <armadillo> // http://arma.sourceforge.net/docs.html v10.2.0
#include <vector>
#include <thread> /* std::this_thread::sleep_for */
namespace EvoAlg
{

    class EAController;

    class EvolutionaryAlgorithm
    {
    public:
        EvolutionaryAlgorithm(EAController &eaController);
        ~EvolutionaryAlgorithm();
        void startAlgorithm();
        void saveBestIndvParamsCSV();
        void initializePop(int);
        void normalizeFitness(double *);
        void evaluatePop();
        void crossover(int, arma::rowvec, arma::rowvec);
        void elitism();
        void tournament();
        void roulette();
        void mutate(int);
        void selectionAndMutation();
        void createCSV(std::string);
        std::string formatParamsString(int);
        void saveGenerationData(int);
        void increaseMutation();
        void predationOfOne();
        void partIncrease();
        void oneRemainingPopReset();
        void endEABatch();
        bool eventHappens(int);
        void checkEvents();
        void startEventTriggerList();
        void evoAlg(int,std::string);
        void fillInitialsWithBests(int);
        void semiFinalsTournment(int);
        void finalTournment();
        void scriptLoop();
        void calcFitness(const std::vector<IndividualRunResult> &);

    private:
        pthread_t scriptThread;
        pthread_mutex_t mutex;
        pthread_mutex_t mutex2;

        arma::rowvec bestIndividual;
        EAController &m_EAController;

        double indvFitness;
        bool eaFinished;
        const double MIN_MULT = 1.0;
        const double MAX_MULT = 1.0;
        const double MUTATE_POSITIVE_MULT = 1.0;
        const double MUTATE_NEGATIVE_MULT = -1.0;

#define NB_PARAMETERS 27 // number of parameters that will be adjusted
#define POPULATION_SIZE 20
#define TOURNMENT_RATE 10

        const long MAX_MUTATION_RATE = 1;
        const double MUTATION_INCREASE_RATIO = 1.1; // mutation increase 10% at a time
        const double INITIAL_MUTATION = 0.05;

        const long BEGIN_DECAY_THRESHOLD = 50; // after how many generations without improvement the mutation starts to decay
        const long APPLY_DECAY_MODULE = 15;    // decay will be applied when the generation index % APPLY_DECAY_MODULE = 0

        const long APPLY_PREDATION_INTERVAL = 10; // interval (in generations) to apply predation

        enum tournments
        {
            SEMI_FINALS,
            FINALS,
            INITIALS
        };

        arma::mat::fixed<TOURNMENT_RATE, NB_PARAMETERS> bestTournmentIndv[2] = {
            arma::mat::fixed<TOURNMENT_RATE, NB_PARAMETERS>(),
            arma::mat::fixed<TOURNMENT_RATE, NB_PARAMETERS>()};

        int eventTriggerModule[6] = {1, 10, 5, 10, 2, 10};

       
        struct GeneValueBound {
            std::string name;
            double min = 0,max = 1;
        };
        #define EA_CB_3(NAME,MIN,MAX) {NAME,MIN,MAX}
        #define EA_CB_2(NAME,MIN) {NAME,MIN,15*MIN}
        #define EA_CB_1(NAME) {NAME,0,1}
        #define EA_GET_MACRO_NAME_3(arg1, arg2, arg3, macro, ...) macro
        #define EA_CB(...) EA_GET_MACRO_NAME_3(__VA_ARGS__,EA_CB_3,EA_CB_2,EA_CB_1)(__VA_ARGS__)



        std::vector<GeneValueBound> attributesValues = {
            EA_CB("ON_ATTACK_MULTIPLIER",1), //DEFAULT
            EA_CB("POP_INCREASE_TAX",1), //DEFAULT
            EA_CB("POP_PER_CONSTRUCTION",1), //DEFAULT
            EA_CB("INIT_POP_VALUE",100), //DEFAULT
            EA_CB("INIT_STAT_VALUE",50), //DEFAULT
            EA_CB("ON_ATTACK_DECAY_TAX"), //DEFAULT
            EA_CB("NORMAL_DECAY_TAX",1), // NORMAL_DECAY_TAX = ON_ATTACK_DECAY_TAX/NORMAL_DECAY_TAX
            EA_CB("ATTACK_FREQUENCY",5), //DEFAULT
            EA_CB("INIT_RESOURCES_VALUE",25), //DEFAULT
            EA_CB("TAX_REDUCT"), //DEFAULT
            EA_CB("MIN_LOSS_0",1), // MIN_LOSS_0 = MAX_LOSS_0/MIN_LOSS_0 
            EA_CB("MIN_LOSS_1",1), // MIN_LOSS_1 = MAX_LOSS_1/MIN_LOSS_1 
            EA_CB("MIN_LOSS_2",1), // MIN_LOSS_2 = MAX_LOSS_2/MIN_LOSS_2 
            EA_CB("MIN_LOSS_3",1), // MIN_LOSS_3 = MAX_LOSS_3/MIN_LOSS_3 
            EA_CB("MAX_LOSS_0"), //DEFAULT
            EA_CB("MAX_LOSS_1"), //DEFAULT
            EA_CB("MAX_LOSS_2"), //DEFAULT
            EA_CB("MAX_LOSS_3"), //DEFAULT
            EA_CB("TOT_ROBOTS_INI",1,25), //DEFAULT
            EA_CB("PROD_COST_INI",5), //DEFAULT
            EA_CB("PROD_COST_INCREASE_TAX"), //DEFAULT
            EA_CB("TIME_STEP",15,60), //DEFAULT
            EA_CB("INIT_TIME_STEP",1), //DEFAULT
            EA_CB("MAX_TIME_STEPS",1), //DEFAULT
            EA_CB("MIN_REWARD",1), //DEFAULT
            EA_CB("REWARD_RANGE",1), //DEFAULT
            EA_CB("FAILURE_TAX") //DEFAULT
        };

        enum selectionMethods
        {
            TOURNAMENT,
            ELITISM,
            ROULETTE
        };
        selectionMethods partsSelectionMethods[20] = {
            TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT, TOURNAMENT,
            ELITISM, ELITISM, ELITISM, ELITISM, ELITISM, ELITISM, ELITISM, ELITISM, ELITISM, ELITISM};

        int partPos = 0;
        selectionMethods selectionMethod = partsSelectionMethods[partPos];

        arma::mat::fixed<POPULATION_SIZE, NB_PARAMETERS> population; // arma::mat : armadillo matrix (Mat<double>)

        // Each LINE (ROW) is the individual's parameters
        double fitness[POPULATION_SIZE];
        double bestFitness = 2000000000, worstFitness = -1;
        int bestFitIndex = -1, worstFitIndex = -1;
        double mutationRate = INITIAL_MUTATION;
        int nbNoImprovementGens = 0; // number of generations in a row without any improvement (best indiviNdual remains the same)
        int generationIndex = 0;

        // Choose if population evaluation will be according to max fitness value or min fitness
        enum evalTypes
        {
            BY_MIN,
            BY_MAX
        };
        evalTypes EVAL_TYPE = BY_MIN;

        bool continueBatch;
        void (EvolutionaryAlgorithm::*selectionType[3])() = {
            &EvolutionaryAlgorithm::tournament,
            &EvolutionaryAlgorithm::elitism,
            &EvolutionaryAlgorithm::roulette};

        void (EvolutionaryAlgorithm::*eventTypes[5])() = {
            &EvolutionaryAlgorithm::increaseMutation,
            &EvolutionaryAlgorithm::predationOfOne,
            &EvolutionaryAlgorithm::partIncrease,
            &EvolutionaryAlgorithm::oneRemainingPopReset,
            &EvolutionaryAlgorithm::endEABatch};
        int remainingFitnessToCalc;
    };

} // namespace EvoAlg
