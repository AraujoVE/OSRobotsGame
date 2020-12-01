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
        void setFood(int);
        int getHealth();
        void setHealth(int);
        int getStructures();
        void setStructures(int);
        int getDefenses();
        void setDefenses(int);
        int getResources();
        void setResources(int);
        int getPopulation();

        void initializeStats();
        void calcNewPop();
};