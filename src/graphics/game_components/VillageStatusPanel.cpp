#include "VillageStatusPanel.hpp"

//TODO: panel size according to game window
VillageStatusPanel::VillageStatusPanel(VillageStats *villageStats) : WPanel(0,0,1000,20) { 
    this->villageStats = villageStats;
}

void VillageStatusPanel::mount() {
    
}

void VillageStatusPanel::render(SDL_Renderer *renderer) {
    this->WPanel::render(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);

}