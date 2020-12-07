#include "Game.hpp"
#include "GameGraphics.hpp"
#include "EventListener.hpp"

#include <iostream>

EventListener::EventListener(const Game& game, SDL_Window *window): game(game) {
    this->window = window;
    this->listening = false;
}

EventListener::~EventListener() {
    if (listening) stopListening();
}

//TODO: encerrar gracefully
void *EventListener::threadLoop(void *gamePtr) {
    Game& game = *(Game*)gamePtr;
    while(true) {
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    std::cout << "Requisitado para sair!!" << std::endl;
                    game.requestGameStop();
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    std::cout << "Cliquei com o mouse em (" << x << ',' << y << ')' << std::endl;
                    game.getGraphics()->getMainPanel()->getEventHandler();

                    break;
                }
                case SDL_MOUSEMOTION: {
                    auto a = e.motion;
                }
                default:
                    break;

            }
        }
        SDL_Delay(100);
    }
    return NULL;
}

void EventListener::startListening() {
    if (listening) return;

    listening = true;
    pthread_create(&thread, NULL, threadLoop, (void*)&game);
}

void EventListener::stopListening() {
    if (!listening) return;

    listening = false;
    pthread_cancel(thread);
    pthread_join(thread, NULL);
}