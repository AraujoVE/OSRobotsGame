#ifndef VILLAGE_STATS
#define VILLAGE_STATS

class VillageStats{
    private:
        const int MAX_STAT_VALUE = 5; // each village stat is between 0 and 6 (included)
        const int INIT_POP_VALUE = 1000;
        const int INIT_RESOURCES_VALUE = 100;

        int defenses;
        int food;
        int health;
        int structures;
        int resources;
        int population; // if population reaches zero, the game is over -> pop calculated based on other village stats

    public:
        VillageStats();

        int getFood() const;
        int getHealth() const;
        int getStructures() const;
        int getDefenses() const;
        int getResources() const;
        int getPopulation() const;

        void setFood(int);
        void setHealth(int);
        void setStructures(int);
        void setDefenses(int);
        void setResources(int);

        void initializeStats();
        void calcNewPop();
        void addTaskResources(RobotFunctions, time_t, int);
};
#endif