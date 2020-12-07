#pragma once

#include "Widget.hpp"
#include <string>

class WImage: public Widget {
    private:
        std::string imagePath;
        SDL_Texture *texture;
    public:
        WImage(SDL_Renderer *renderer, const std::string& imagePath);
        virtual ~WImage();


        void setImage(const std::string& imagePath);


        void render() const;
};