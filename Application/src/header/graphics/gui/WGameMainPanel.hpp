#pragma once

#include "Widget.hpp"
#include "Game.fwd.hpp"

class WGameMainPanel : public Widget {
    private:
        const Game& game;
    
    public:
        WGameMainPanel(SDL_Renderer *renderer, const Game& game);
        void mount();
};