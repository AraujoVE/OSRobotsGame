#include <stdexcept>
#include <iostream>

#include "GameGraphics.hpp"
#include "Game.hpp"
#include "GameMainPanel.hpp"

GameGraphics::GameGraphics(const Game &game, SDL_Window* window): game(game) {
    setWindow(window);
    this->mainPanel = new GameMainPanel(renderer, game);
    this->eventListener = new EventListener(game, window);
}

void GameGraphics::init() {
    this->mainPanel->mount();
    this->eventListener->startEventListening();    
}

GameGraphics::~GameGraphics() {
    delete eventListener;
    delete mainPanel;
}

void GameGraphics::setWindow(SDL_Window* window) {
    SDL_Renderer *renderer = SDL_GetRenderer(window);
    this->window = window;
    this->renderer = renderer;
}

void GameGraphics::renderGame() const {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    getMainPanel()->render();
    SDL_RenderPresent(renderer);
}

WPanel *GameGraphics::getMainPanel() const { 
    return mainPanel; 
}

SDL_Window *GameGraphics::getWindow() const {
    return window;
}

SDL_Renderer *GameGraphics::getRenderer() const {
    return renderer;
}
