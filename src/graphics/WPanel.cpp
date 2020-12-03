#include "WPanel.hpp"

WPanel::WPanel(int x, int y, int w, int h) {
    this->rect = {x, y, w, h};
}

void WPanel::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &this->rect);
    SDL_RenderPresent(renderer);
}