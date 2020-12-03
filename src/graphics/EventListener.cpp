#include "Game.hpp"
#include "GameGraphics.hpp"
#include "EventListener.hpp"

#include <iostream>

EventListener::EventListener(const Game& game, SDL_Window *window): game(game) {
    this->window = window;
    this->listening = false;
}

EventListener::~EventListener() {
    if (listening) stopEventListening();
}

void *EventListener::threadLoop(void *gamePtr) {
    Game& game = *(Game*)gamePtr;
    while(true) {
        SDL_Event e;
        std::cout << "Tentando..." << std::endl;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    std::cout << "Requisitado para sair!!" << std::endl;
                    game.quit();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    std::cout << "Cliquei com o mouse em (" << x << ',' << y << ')' << std::endl;
                    break;
                default:
                    break;

            }
        }
        SDL_Delay(100);
    }
    return NULL;
}

void EventListener::startEventListening() {
    listening = true;
    pthread_create(&thread, NULL, threadLoop, (void*)&game);
}

void EventListener::stopEventListening(){
    listening = false;
    pthread_cancel(thread);
    pthread_join(thread, NULL);
}