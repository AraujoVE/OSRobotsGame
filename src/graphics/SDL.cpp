#include "SDL.hpp"


namespace OSDL {
    bool initialized = false;

    void initModules() {
        if (initialized) return;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error(std::string("error initializing SDL:") + SDL_GetError());
        }

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            throw std::runtime_error(std::string("error initializing SDL_image's PNG module: ") + IMG_GetError());
        }

        if (TTF_Init() != 0) {
            throw std::runtime_error(std::string("error initializing SDL_ttf module: ") + TTF_GetError());
        }

        initialized = true;
    }

    void stopModules() {
        if (!initialized) throw std::logic_error("Trying to stop modules before initializing SDL");
        SDL_Quit();
        IMG_Quit();
        TTF_Quit();
    }

    SDL_Window *createWindowWithRender(const std::string& title) {
        if (!initialized) throw std::logic_error("Trying to create window before initializing SDL");

        SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
        if (window == nullptr) { throw std::runtime_error("Unable to create SDL Window"); }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr) { throw std::runtime_error("Unable to create SDL Renderer"); }


        return window;  
    }
};