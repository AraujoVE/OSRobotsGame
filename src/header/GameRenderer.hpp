#pragma once

#include "SDL.hpp"
#include "Game.hpp"
class Game;

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