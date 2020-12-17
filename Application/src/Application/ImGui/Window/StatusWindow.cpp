#include "StatusWindow.hpp"

namespace Application
{
    void *spawnChange(void *abab) {
        // DE_DEBUG("Inside new thread, spawning change");
        sleep(rand()%2+1);
        ((VillageStats*)abab)->changeStat(VillageStats::POPULATION_INDEX, +2000);
        return NULL;
    }
} // namespace Application


