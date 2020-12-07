#pragma once

#include "Core/Base.hpp"

namespace DampEngine
{

    enum class EventType {
        //Key related
        KeyPressed, KeyReleased, KeyTyped, 

        //Mouse related
        MouseEnterWindow, MouseLeaveWindow,
        MouseMoved, MouseButtonPressed, MouseButtonReleased,
        MouseScrolled,

        //WindowRelated
        WindowClose, WindowFocus, WindowBlur, 
        WindowMinimize, WindowMaximize, WindowRestore,
        WindowMoved, WindowRefreshed, WindowResized, WindowContentScaled,
        
        //Fullscreen
        MonitorConnected, MonitorDisconnected,

        //Joystick
        JoystickConnected, JoystickDisconnected
    };

    enum class EventCategory {
        Mouse           = BIT(1),
        Keyboard        = BIT(2),
        Joystick        = BIT(3),

        WindowTransform = BIT(4),
        WindowInput     = BIT(5),

        Monitor         = BIT(6)
    };

    // enum class EventCategoryFilter {
    //     Input           = EventCategory::Mouse | EventCategory::Keyboard | EventCategory::Joystick,
    //     Window          = EventCategory::WindowTransform | EventCategory::WindowInput,
    // };

    class Event
    {
    public:
        friend class EventDispatcher;
     
        virtual EventType getEventType() const = 0;
        // virtual EventCategory getEventCategory() = 0;
    private:
        bool m_Handled = false;
    };

    template<class E>
    class EventDispatcher
    {
    public:

        using EventCallbackFn = std::function<void(E &)>;

        static void Dispatch(const E& e, EventCallbackFn callback) {
            if (e.getEventType() == E.getEventType()) {
                callback(e);
            }
        }
    };

} // namespace DampEngine
