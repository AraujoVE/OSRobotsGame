#pragma once

#include <string>
#include "Widget.hpp"

//TODO: maybe a TexturedWidget? (WImage has a lot of things in common with this class)
class WText : public Widget {
    private:
        void mount();
        void render() const;
        SDL_Texture *texture;
        OSDL::Font font;
    public:
        WText(SDL_Renderer *renderer, const std::string& text);
        ~WText();
        void setText(const std::string& text);
};