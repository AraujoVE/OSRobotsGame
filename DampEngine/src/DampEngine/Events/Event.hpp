#pragma once

#include "DampEngine/Core/Base.hpp"

#include <functional>

namespace DampEngine
{

    enum class EventType {
        //Key related
        KeyPressed, KeyReleased, KeyTyped, 

        //Mouse related
        MouseEnterWindow, MouseLeaveWindow, MouseMoved,
        MouseButtonPressed, MouseButtonReleased,
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
    protected:
        explicit Event()
        {
            m_Handled = false;
        }

    public:
        virtual ~Event() {}
        friend class EventDispatcher;

        static EventType GetStaticEventType();
        inline EventType GetEventType() const { return GetStaticEventType(); }
        inline bool IsHandled() const { return m_Handled; }

        // virtual EventCategory getEventCategory() = 0;
    private:
        bool m_Handled = false;
    };

    class EventDispatcher
    {
    public:

        template<class E>
        using EventCallbackFn = std::function<bool(E &)>;

        template<class E>
        static void Dispatch(const Event& event, EventCallbackFn<E> onevent_fn) {
            if (event.m_Handled) return;

            if (event.GetEventType() == E::GetEventType()) {
                bool handled = onevent_fn(event);
                if (handled) event.m_Handled = true; 
            }
        }
    };

} // namespace DampEngine
