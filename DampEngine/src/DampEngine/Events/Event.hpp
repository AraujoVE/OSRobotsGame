#pragma once

#include "DampEngine/Core/Macros/Events.hpp"
#include "DampEngine/Core/Macros/Primitives.hpp"

#include <string>
#include <functional>

#define DE_INTERNAL_EVENT_DEFINITIONS() \
        KeyPressed,\
        KeyReleased,\
        KeyTyped,\
        MouseEnteredWindow,\
        MouseLeftWindow,\
        MouseMoved,\
        MouseButtonPressed,\
        MouseButtonReleased,\
        MouseScrolled,\
        WindowClosed,\
        WindowFocused,\
        WindowBlurred,\
        WindowMinimized,\
        WindowMaximized,\
        WindowRestored,\
        WindowMoved,\
        WindowRefreshed,\
        WindowResized,\
        WindowContentScaled,\
        MonitorConnected,\
        MonitorDisconnected,\
        JoystickConnected,\
        JoystickDisconnected

// #define DE_INTERNAL_APPEND_PREF(event) EventType::event
// #define DE_INTERNAL_EVENTS_LIST() MAP(DE_INTERNAL_APPEND_PREF, DE_INTERNAL_EVENT_DEF())

namespace DampEngine
{
    enum class EventType
    {
        Invalid,
        DE_INTERNAL_EVENT_DEFINITIONS()
    };

    class Event
    {
    protected:
        explicit Event() {}

    public:
        friend class EventDispatcher;

        virtual ~Event(){};

        inline static EventType GetStaticEventType() { return EventType::Invalid; }
        inline static const std::string GetStaticEventName() { return "Invalid"; }

        virtual EventType GetEventType() const = 0;
        virtual std::string GetEventName() const = 0;

        inline bool IsHandled() const { return m_Handled; }

        virtual std::string ToString() const;
        // virtual EventCategory getEventCategory() = 0;
    private:
        bool m_Handled = false;
    };

    class VBlankEvent : public Event
    {
    public:
        VBlankEvent() : Event() {

        }
        
        virtual std::string ToString() const override { return GetEventName(); };
    };

    class EventDispatcher
    {
    public:
        template <class E>
        using EventCallbackFn = std::function<bool(E &)>;

        template <class E>
        static void Dispatch(Event &event, EventCallbackFn<E> onevent_fn)
        {
            if (event.m_Handled)
                return;

            if (event.GetEventType() == E::GetStaticEventType())
            {
                bool handled = onevent_fn(*(E *)&event);
                if (handled)
                    event.m_Handled = true;
            }
        }
    };

} //namespace DampEngine

std::ostream &operator<<(std::ostream &os, const DampEngine::Event &event);
std::ostream &operator<<(std::ostream &os, const DampEngine::EventType &event_type);
