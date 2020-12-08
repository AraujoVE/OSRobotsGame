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

} // namespace DampEngine