#pragma once

#include "DampEngine/Core/Macros/Events.hpp"

#include <string>
#include <functional>

namespace DampEngine
{
    enum class EventType {
        Invalid,
        //Key related
        KeyPressed, KeyReleased, KeyTyped, 

        //Mouse related
        MouseEnteredWindow, MouseLeftWindow, MouseMoved,
        MouseButtonPressed, MouseButtonReleased,
        MouseScrolled,

        //WindowRelated
        WindowClosed, WindowFocused, WindowBlurred, 
        WindowMinimized, WindowMaximized, WindowRestored,
        WindowMoved, WindowRefreshed, WindowResized, WindowContentScaled,
        
        //Fullscreen
        MonitorConnected, MonitorDisconnected,

        //Joystick
        JoystickConnected, JoystickDisconnected
    };

    // enum class EventCategory {
    //     Mouse           = BIT(1),
    //     Keyboard        = BIT(2),
    //     Joystick        = BIT(3),

    //     WindowTransform = BIT(4),
    //     WindowInput     = BIT(5),

    //     Monitor         = BIT(6)
    // };

    // enum class EventCategoryFilter {
    //     Input           = EventCategory::Mouse | EventCategory::Keyboard | EventCategory::Joystick,
    //     Window          = EventCategory::WindowTransform | EventCategory::WindowInput,
    // };

    class Event
    {
    protected:
        explicit Event() {}

    public:
        friend class EventDispatcher;
        
        virtual ~Event() {};

        inline static EventType GetStaticEventType() { return EventType::Invalid; }
        inline static const std::string GetStaticEventName() { return "Invalid"; }

        virtual inline EventType GetEventType() const = 0;
        virtual inline std::string GetEventName() const = 0;
        
        inline bool IsHandled() const { return m_Handled; }

        virtual std::string ToString() const;
        // virtual EventCategory getEventCategory() = 0;
    private:
        bool m_Handled = false;
    };

    class VBlankEvent : public Event {
    public:
        VBlankEvent(): Event() {}
        std::string ToString() const override { return GetEventName(); };
    };

    class EventDispatcher
    {
    public:

        template<class E>
        using EventCallbackFn = std::function<bool(E &)>;

        template<class E>
        static void Dispatch(Event& event, EventCallbackFn<E> onevent_fn) {
            if (event.m_Handled) return;

            if (event.GetEventType() == E::GetStaticEventType()) {
                bool handled = onevent_fn(*(E*)&event);
                if (handled) event.m_Handled = true; 
            }
        }
    };

} //namespace DampEngine 

std::ostream &operator<<(std::ostream &os, const DampEngine::Event &event);
std::ostream &operator<<(std::ostream &os, const DampEngine::EventType &event_type);

