#pragma once

#include <functional>
#include "Vec2D.hpp"
#include "SDL.hpp"

using namespace OUtil;

namespace EventHandlerCallbacks {
    typedef std::function<bool(const Vec2D& pos, const SDL_MouseButtonEvent& e)> MouseClickEventCallback;
    typedef std::function<bool(const Vec2D& pos, const SDL_MouseMotionEvent& e)> MouseMovementEventCallback;
};

using namespace EventHandlerCallbacks;

/**
 *  
 * 
 */
class EventHandler {
    private:
        MouseClickEventCallback mouseDownCallback;
        MouseClickEventCallback mouseUpCallback;
        MouseMovementEventCallback mouseMovedCallback;
        MouseMovementEventCallback mouseHoverInCallback;
        MouseMovementEventCallback mouseHoverOutCallback;
    public:
        EventHandler();
        ~EventHandler();
        
        void setOnMouseDownCallback(MouseClickEventCallback callback);
        void setOnMouseUpCallback(MouseClickEventCallback callback);
        void setOnMouseMovedCallback(MouseMovementEventCallback callback);
        void setOnMouseHoverInCallback(MouseMovementEventCallback callback);
        void setOnMouseHoverOutCallback(MouseMovementEventCallback callback);
};

// void Widget::onClick(int x, int y) const{
//     if (x < transform.x || x > transform.x + transform.w ) return;
//     if (y < transform.y || y > transform.y + transform.h ) return;

//     printf("Widget %p received click! at (%d, %d)\n", this, x, y);

//     bool passToChildren = true;
//     if (mouseDownCallback != nullptr)
//         passToChildren = mouseDownCallback({x, y});

//     if (passToChildren) {
//         for (auto child: innerWidgets) {
//             child->onClick(x, y);
//         }
//     }
// }