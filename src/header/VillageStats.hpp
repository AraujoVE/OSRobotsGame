#ifndef VILLAGE_STATS
#define VILLAGE_STATS

class VillageStats{
    private:
        const int maxStatValue = 5; // each village stat is between 0 and 6 (included)
        const int initPopValue = 1000;
        const int initResourcesValue = 100;

        int defenses;
        int food;
        int health;
        int structures;
        int resources;
        int population; // if population reaches zero, the game is over -> pop calculated based on other village stats

    public:
        VillageStats();

        int getFood();
        int getHealth();
        int getStructures();
        int getDefenses();
        int getResources();
        int getPopulation();

        void setFood(int);
        void setHealth(int);
        void setStructures(int);
        void setDefenses(int);
        void setResources(int);

        void initializeStats();
        void calcNewPop();
};
#endif