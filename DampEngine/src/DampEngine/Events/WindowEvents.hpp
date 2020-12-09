#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class WindowClosedEvent : public Event
    {
    public:
        using Event::Event;

        std::string ToString() const override {
            return GetEventName();
        }

        DE_EVENT_SET_TYPE(WindowClosed);
    };

    class WindowResizedEvent : public Event
    {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowResized);
    };


    class WindowFocusedEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowFocused);
    };

    class WindowBlurredEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowBlurred);
    };

    class WindowMinimizedEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowMinimized);
    };

    class WindowMaximizedEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowMaximized);
    };

    class WindowRestoredEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowRestored);
    };

    class WindowMovedEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowMoved);
    };

    class WindowRefreshedEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowRefreshed);
    };

    class WindowContentScaledEvent : Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowContentScaled);
    };

} // namespace DampEngine