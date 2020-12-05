#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>

#include <stdexcept>

namespace OSDL {
    void initModules();
    void stopModules();
    SDL_Window *createWindowWithRender(const std::string& title);
};