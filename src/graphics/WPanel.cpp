#include "WPanel.hpp"

#include <stdio.h>

WPanel::WPanel(int x, int y, int w, int h) {
    this->rect = {x, y, w, h};
    this->clickCallback = nullptr;
    mount();
}

WPanel::~WPanel() {
    for (auto item: innerPanels) {
        delete item;
    }
}

void WPanel::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderDrawRect(renderer, &rect);
    for (auto item: innerPanels) {
        item->render(renderer);
    }
}

void WPanel::setOnClick(OnClickCallback callback) {
    this->clickCallback = callback;
}

void WPanel::onClick(int x, int y) {
    if (x < rect.x || x > rect.x + rect.w ) return;
    if (y < rect.y || y > rect.y + rect.h ) return;

    printf("WPanel %p received click! at (%d, %d)\n", this, x, y);

    bool passToChildren = true;
    if (clickCallback != nullptr)
        passToChildren = clickCallback(x, y);

    if (passToChildren) {
        for (auto child: innerPanels) {
            child->onClick(x, y);
        }
    }
}