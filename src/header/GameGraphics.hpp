#pragma once

#include "SDL.hpp"
#include "Game.fwd.hpp"
#include "GameGraphics.fwd.hpp"

#include "WPanel.hpp"
#include "EventListener.hpp"

class GameGraphics {
private:
    const Game& game;
    SDL_Renderer *renderer;
    SDL_Window *window;

    WPanel *mainPanel;
    EventListener *eventListener;
public:
    void setWindow(SDL_Window *window);

    void renderGame() const;

    GameGraphics(const Game &game, SDL_Window* window);
    ~GameGraphics();

    WPanel *getMainPanel() const;
    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;

};