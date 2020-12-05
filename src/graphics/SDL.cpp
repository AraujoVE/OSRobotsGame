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

    TTF_Font *loadFont(const std::string& path, int point_size) {
        if (!initialized) throw std::logic_error("Trying to load font before initializing SDL_ttf");


        TTF_Font *ttf_font = TTF_OpenFont(path.c_str(), point_size);
        if (ttf_font == NULL) { throw std::runtime_error("error: font not found: " + path + "\n\tmsg: " + TTF_GetError()); }
        return ttf_font;
    }


    FontWrapper::FontWrapper(const std::string& path, int point_size) {
        this->ttf_font = loadFont(path, point_size);
    }

    FontWrapper::~FontWrapper() {
        TTF_CloseFont(ttf_font);
    }

    TTF_Font *FontWrapper::getSDLFont() const {
        return ttf_font;
    }

    std::shared_ptr<FontWrapper> useFont(const std::string& path, int point_size) {
        return std::make_shared<FontWrapper>(path, point_size);
    }

};