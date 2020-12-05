#pragma once

#include "SDL.hpp"
#include <vector>

//Passing x, y and returns if the event should be passed to children elements
typedef bool(*OnClickCallback)(int x, int y);

class Widget {
    protected:
        SDL_Renderer *renderer;
        SDL_Rect transform;
        std::vector<Widget*> innerWidgets;
        OnClickCallback clickCallback;
    public:
        Widget(SDL_Renderer *renderer, const SDL_Rect& transform);
        Widget(SDL_Renderer *renderer);
        virtual ~Widget();

        virtual void mount() { }

        void setTransform(const SDL_Rect& transform);
        SDL_Rect getTransform() const;

        virtual void render() const;

        void setOnClick(OnClickCallback callback);
        void onClick(int x, int y) const;


};