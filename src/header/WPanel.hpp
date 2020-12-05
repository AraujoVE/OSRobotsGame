#pragma once

#include "SDL.hpp"
#include <vector>

//Passing x, y and returns if the event should be passed to children elements
typedef bool(*OnClickCallback)(int x, int y);

class WPanel {
    protected:
        SDL_Rect rect;
        std::vector<WPanel*> innerPanels;
        void clear();
        OnClickCallback clickCallback;
    public:
        WPanel(int x, int y, int w, int h);
        virtual ~WPanel();

        void setOnClick(OnClickCallback callback);
        virtual void render(SDL_Renderer *renderer);

        virtual void mount() { }

        void onClick(int x, int y);

};