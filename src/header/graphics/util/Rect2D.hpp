#pragma once

#include "SDL.hpp"
#include "Vec2D.hpp"

namespace OUtil {
    struct Rect2D {
        Vec2D position, dimension;

        Rect2D(const Vec2D& position, const Vec2D& dimension): position(position), dimension(dimension) {}
        Rect2D(int x, int y, int w, int h): Rect2D({x,y}, {w,h}) {}
        Rect2D(const SDL_Rect& rect): Rect2D(rect.x, rect.y, rect.w, rect.h) {}

        SDL_Rect toSDLRect() { return {position.x, position.y, dimension.x, dimension.y }; }

        bool containsPoint(const Vec2D& point) { 
            Vec2D startToPointV = (point - position).getQuadrant();
            Vec2D endToPointV = (point - (position + dimension)).getQuadrant();
        }

        bool isCompletelyContainedIn(const Rect2D& otherRect) {
            return otherRect.containsPoint(position) && otherRect.containsPoint(position+dimension);
        }

    };
}