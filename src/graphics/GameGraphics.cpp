#include <stdexcept>

#include "GameGraphics.hpp"
#include "Game.hpp"

#include "SDL.hpp"

#include "WGameMainPanel.hpp"

GameGraphics::GameGraphics(const Game &game): game(game) {}

void GameGraphics::start() {
    setWindowAndRenderer(OSDL::createWindowWithRender(WINDOW_TITLE));
    rendering = true;

    this->mainPanel = new WGameMainPanel(renderer, game);
    this->mainPanel->mount();
    
    this->eventListener = new EventListener(game, window);
    this->eventListener->startListening();
}

void GameGraphics::stop() {
    rendering = false;

    delete mainPanel;
    delete eventListener;

    SDL_DestroyWindow(window);

    this->window = nullptr;
    this->renderer = nullptr;
}

GameGraphics::~GameGraphics() {
    if (rendering) stop();
}

void GameGraphics::setWindowAndRenderer(SDL_Window* window) {
    SDL_Renderer *renderer = SDL_GetRenderer(window);
    this->window = window;
    this->renderer = renderer;
}

void GameGraphics::renderGame() const {
    //Clear screen with white
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    //Render main panel and it's components
    getMainPanel()->render();

    //Present staged changes to the screen (fflush-like operation)
    SDL_RenderPresent(renderer);
}

WGameMainPanel *GameGraphics::getMainPanel() const { 
    return mainPanel; 
}

SDL_Window *GameGraphics::getWindow() const {
    return window;
}

SDL_Renderer *GameGraphics::getRenderer() const {
    return renderer;
}
