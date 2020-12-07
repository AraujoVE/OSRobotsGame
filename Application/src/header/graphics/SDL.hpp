#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>
#include <memory>

namespace OSDL {
    namespace FontPaths {
        const std::string OPEN_SANS = "/home/marucs/Development/USP/BCC/2020-2/SO/OSRobotsGame/assets/OpenSans-Regular.ttf";
    }

    class FontWrapper {
        private:
            TTF_Font *ttf_font;
        public:
            FontWrapper(const std::string& path, int size);
            ~FontWrapper();
            TTF_Font *getSDLFont() const;
            friend std::shared_ptr<FontWrapper> useFont(const std::string& path, int size);
    };

    typedef std::shared_ptr<FontWrapper> Font;
    std::shared_ptr<FontWrapper> useFont(const std::string& path, int size);
    
    
    void initModules();
    void stopModules();
    SDL_Window *createWindowWithRender(const std::string& title);
};