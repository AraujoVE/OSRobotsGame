#include "WPanel.hpp"
#include "VillageStats.hpp"

class VillageStatusPanel : public WPanel {
    private: 
        VillageStats *villageStats;

    public:
        VillageStatusPanel(SDL_Renderer *renderer, VillageStats* villageStats);

        void mount();
        void render() const;
};