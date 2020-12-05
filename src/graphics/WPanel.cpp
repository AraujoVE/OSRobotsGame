#include "WPanel.hpp"

#include <stdio.h>

WPanel::WPanel(SDL_Renderer *renderer): WPanel(renderer, {0,0,0,0}) {}

WPanel::WPanel(SDL_Renderer *renderer, const SDL_Rect& transform) {
    this->clickCallback = nullptr;
    this->renderer = renderer;
    setTransform(transform);
}

WPanel::~WPanel() {
    for (auto item: innerPanels) {
        delete item;
    }
}

void WPanel::setTransform(const SDL_Rect& transform) {
    this->transform = transform;
}


SDL_Rect WPanel::getTransform() const {
    return this->transform;
}

void WPanel::render() const{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &transform);
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderDrawRect(renderer, &transform);
    for (auto item: innerPanels) {
        item->render();
    }
}

void WPanel::setOnClick(OnClickCallback callback) {
    this->clickCallback = callback;
}

void WPanel::onClick(int x, int y) const{
    if (x < transform.x || x > transform.x + transform.w ) return;
    if (y < transform.y || y > transform.y + transform.h ) return;

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