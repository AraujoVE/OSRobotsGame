#pragma once

#include "Widget.hpp"
#include "VillageStats.hpp"

class WVillageStatusPanel : public Widget {
    private: 
        VillageStats *villageStats;

    public:
        WVillageStatusPanel(SDL_Renderer *renderer, VillageStats* villageStats);

        void mount();
        void render() const;
};