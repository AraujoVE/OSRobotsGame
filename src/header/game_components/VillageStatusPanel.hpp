#include "WPanel.hpp"
#include "VillageStats.hpp"

class VillageStatusPanel : public WPanel {
    private: 
        VillageStats *villageStats;

    public:
        VillageStatusPanel(VillageStats* villageStats);

        void mount();
        void render(SDL_Renderer *renderer);
};