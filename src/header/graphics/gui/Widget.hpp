#pragma once

#include "Vec2D.hpp"
#include "SDL.hpp"
#include <vector>
#include <functional>
#include "EventHandler.hpp"

//Every 2D GUI element is a Widget, this class is their root class
class Widget {
    protected:
        SDL_Renderer *renderer;
        SDL_Rect transform;
        std::vector<Widget*> innerWidgets;
        EventHandler *eventHandler;
        
        virtual void mount();
        virtual void update();
        virtual void render() const;
    public:
        Widget(SDL_Renderer *renderer, const SDL_Rect& transform = {0,0,0,0});
        virtual ~Widget();

        EventHandler *getEventHandler();

        void setTransform(const SDL_Rect& transform);
        inline SDL_Rect getTransform() const;

        void setPosition(const Vec2D& position);
        inline Vec2D getPosition() const;

        void setDimention(const Vec2D& position);
        inline Vec2D getDimension() const;

        void requestMount();
        void requestUpdate();
        void requestRender(bool renderChildren = true) const;
};