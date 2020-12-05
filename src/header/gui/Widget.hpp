#pragma once

#include "SDL.hpp"
#include <vector>
#include <functional>

//Passing x, y and returns if the event should be passed to children elements
typedef std::function<bool(int,int)> OnClickCallback;

class Widget {
    protected:
        SDL_Renderer *renderer;
        SDL_Rect transform;
        std::vector<Widget*> innerWidgets;
        OnClickCallback clickCallback;

        virtual void mount();
        virtual void update();
        virtual void render() const;
    public:
        Widget(SDL_Renderer *renderer, const SDL_Rect& transform);
        Widget(SDL_Renderer *renderer);
        virtual ~Widget();


        void setTransform(const SDL_Rect& transform);
        SDL_Rect getTransform() const;

        void requestMount();
        void requestUpdate();
        void requestRender(bool renderChildren = true) const;

        void setOnClick(OnClickCallback callback);
        void onClick(int x, int y) const;


};