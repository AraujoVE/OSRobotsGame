#include "header/VillageStats.hpp"
#include "header/RobotsManagement.hpp"

void startGame() {
        VillageStats * village = new VillageStats(); // initialize village stats
        RobotsManagement * robotsManag = new RobotsManagement();

        return;
}

bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
    if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
        return true;
    return false;
}   

int main(void){
    return 0;
}