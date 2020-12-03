#pragma once

#include "SDL.hpp"
#include <vector>

typedef void(*OnClickCallback)(int x, int y);

class WPanel {
    private:
        SDL_Rect rect;
        std::vector<WPanel> innerPanels;
        void clear();
        OnClickCallback *callback;
    public:
        void setOnClick(OnClickCallback callback);
        void render(SDL_Renderer *renderer);

        void onClick(int x, int y);

        WPanel(int x, int y, int w, int h);
        ~WPanel();
};