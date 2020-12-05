#include "VillageStatusPanel.hpp"

//TODO: panel size according to game window
VillageStatusPanel::VillageStatusPanel(SDL_Renderer *renderer, VillageStats *villageStats) : WPanel(renderer, {0,0,1000,20}) { 
    this->villageStats = villageStats;
}

void VillageStatusPanel::mount() {
    
}

void VillageStatusPanel::render() const{
    this->WPanel::render();
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &transform);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, &transform);
    SDL_RenderPresent(renderer);

}