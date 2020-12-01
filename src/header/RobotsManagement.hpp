enum RobotFunctions{
    PROTECTION,
    RESOURCE_GATHERING,
    HUNT,
    MEDICINE,
    CONSTRUCTION,
};

class RobotsManagement{
    private:
        bool inProgress;
        int totRobots;
        int freeRobots;
        int efficiency;
        int prodCost;
    public:
        bool sendRobot(RobotFunctions funct);
        bool retrieveRobot();
        bool createRobot();
        bool destroyRobot();
};