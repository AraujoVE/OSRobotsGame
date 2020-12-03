#include <stdexcept>

#include "header/GameRenderer.hpp"

GameRenderer::GameRenderer(const Game &game, SDL_Renderer* renderer): game(game) {
    setRenderer(renderer);
}

GameRenderer::~GameRenderer() {

}

void GameRenderer::setRenderer(SDL_Renderer* renderer) {
    if (renderer == nullptr) throw std::logic_error("Attempt to set a null renderer");
    this->currentRenderer = renderer;
}

void GameRenderer::renderGame() const {
    SDL_Rect rec = {10,10,10,10};

    // SDL_SetRenderDrawColor(currentRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    // SDL_RenderFillRect(currentRenderer, &rec);
    // SDL_RenderPresent(currentRenderer);
    // SDL_Delay(500);
}
