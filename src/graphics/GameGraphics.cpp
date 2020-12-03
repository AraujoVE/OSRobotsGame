#include <stdexcept>
#include <iostream>

#include "GameGraphics.hpp"
#include "Game.hpp"

GameGraphics::GameGraphics(const Game &game, SDL_Window* window): game(game) {
    setWindow(window);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    this->mainPanel = new WPanel(0,0, w, h);

    this->eventListener = new EventListener(game, window);
    this->eventListener->startEventListening();    
}

GameGraphics::~GameGraphics() {
    delete eventListener;
}

void GameGraphics::setWindow(SDL_Window* window) {
    SDL_Renderer *renderer = SDL_GetRenderer(window);
    this->window = window;
    this->renderer = renderer;
}

void GameGraphics::renderGame() const {
    getMainPanel()->render(renderer);
}

WPanel *GameGraphics::getMainPanel() const { 
    return mainPanel; 
}

SDL_Window *GameGraphics::getWindow() const {
    std::cout << "TEET" << std::endl;
    return window;
}

SDL_Renderer *GameGraphics::getRenderer() const {
    return renderer;
}
