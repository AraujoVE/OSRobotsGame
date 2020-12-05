#pragma once

#include "SDL.hpp"
#include "Widget.hpp"

class WTaskController : public Widget {
    public:
        WTaskController(SDL_Renderer *renderer);
        void mount();
        void render() const;
};