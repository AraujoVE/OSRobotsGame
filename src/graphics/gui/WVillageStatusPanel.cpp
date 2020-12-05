#include "WVillageStatusPanel.hpp"

//TODO: panel size according to game window
WVillageStatusPanel::WVillageStatusPanel(SDL_Renderer *renderer, VillageStats *villageStats) : Widget(renderer, {0,0,1000,20}) { 
    this->villageStats = villageStats;
}

void WVillageStatusPanel::mount() {
    
}

void WVillageStatusPanel::render() const{
    this->Widget::render();
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &transform);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, &transform);
    SDL_RenderPresent(renderer);

}