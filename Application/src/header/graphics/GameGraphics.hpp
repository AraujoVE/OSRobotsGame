#pragma once

#include "SDL.hpp"
#include "Game.fwd.hpp"
#include "GameGraphics.fwd.hpp"

#include "Widget.hpp"
#include "EventListener.hpp"
#include "WGameMainPanel.hpp"

#define WINDOW_TITLE "OSRobots"

class GameGraphics {
private:
    const Game& game;
    SDL_Renderer *renderer;
    SDL_Window *window;

    WGameMainPanel *mainPanel;
    EventListener *eventListener;

    bool rendering;
public:
    void setWindowAndRenderer(SDL_Window *window);

    void renderGame() const;

    GameGraphics(const Game &game);
    ~GameGraphics();

    void start();
    void stop();

    inline WGameMainPanel *getMainPanel() const;
    inline SDL_Window *getWindow() const;
    inline SDL_Renderer *getRenderer() const;

};