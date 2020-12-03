#pragma once

#include "Game.fwd.hpp"
#include "SDL.hpp"
#include <pthread.h>
#include <unordered_map>

class EventListener {
private:
    SDL_Window *window;
    pthread_t thread;
    const Game& game;
    bool listening;
    
    static void *threadLoop(void *);

public:

    EventListener(const Game& game, SDL_Window *window);
    ~EventListener();

    void startEventListening();
    void stopEventListening();
};