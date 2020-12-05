#pragma once

#include <string>
#include "Widget.hpp"

#define FONT_MACRO TTF_OpenFont("/home/marucs/Development/USP/BCC/2020-2/SO/OSRobotsGame/OpenSans-Regular.ttf", 128); 

//TODO: maybe a TexturedWidget? (WImage has a lot of things in common with this class)
class WText : public Widget {
    private:
        void mount();
        void render() const;
        SDL_Texture *texture;
    public:
        WText(SDL_Renderer *renderer, const std::string& text);
        ~WText();
        void setText(const std::string& text);
};