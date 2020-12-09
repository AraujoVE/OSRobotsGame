#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class JoystickConnectedEvent : public Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(JoystickConnected);
    };

    class JoystickDisconnectedEvent : public Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(JoystickDisconnected);
    };    
} // namespace DampEngine
