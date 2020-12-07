#pragma once

#include "SDL.hpp"
#include "Widget.hpp"
#include "WText.hpp"

class WTaskController : public Widget {
    private:
        WText *wRobotCountText;
        int robotCount;
        void mount();
        void update();
        void render() const;
    public:
        WTaskController(SDL_Renderer *renderer);
        void increaseRobots();
        void decreaseRobots();
};