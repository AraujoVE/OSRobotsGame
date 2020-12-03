#pragma once

#include "SDL.hpp"
#include "Game.fwd.hpp"
#include "GameRenderer.fwd.hpp"

class GameRenderer {
private:
    const Game& game;
    SDL_Renderer *currentRenderer;
public:
    void setRenderer(SDL_Renderer *newRenderer);

    void renderGame() const;

    GameRenderer(const Game &game, SDL_Renderer* renderer);
    ~GameRenderer();
};