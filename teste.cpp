#include <stdlib.h>  /* srand, rand */
#include <iostream>
#include <fstream> // creating (.csv) files
#include <vector>
#include <cstring>
#include <functional>
#include <armadillo>    

#define NB_PARAMETERS 28 // number of parameters that will be adjusted
#define POPULATION_SIZE 20

int main()
{
    FILE* fp;
    fp = fopen("constValues.cfg", "w");

    char valueNames[NB_PARAMETERS][30]{
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
    
    int len;
    for (size_t i = 0; i < NB_PARAMETERS; i++)
    {
        int len = strlen(valueNames[i]);
        fwrite(valueNames[i], len, sizeof(char), fp);
        fwrite(" = ", sizeof(char), 3, fp);
        fwrite("\n", 1, 1, fp);
        printf("k%sk\n", valueNames[i]);
    }

    fclose(fp);
    //population.print("Population matrix initialized:");

    return 0;
}
