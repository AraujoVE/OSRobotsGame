#pragma once

#include "SDL.hpp"
#include "Game.fwd.hpp"
#include "GameGraphics.fwd.hpp"

#include "Widget.hpp"
#include "EventListener.hpp"
#include "WGameMainPanel.hpp"

class GameGraphics {
private:
    const Game& game;
    SDL_Renderer *renderer;
    SDL_Window *window;

    WGameMainPanel *mainPanel;
    EventListener *eventListener;
public:
    void setWindow(SDL_Window *window);

    void renderGame() const;

    GameGraphics(const Game &game, SDL_Window* window);
    ~GameGraphics();

    void init();

    WGameMainPanel *getMainPanel() const;
    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;

};