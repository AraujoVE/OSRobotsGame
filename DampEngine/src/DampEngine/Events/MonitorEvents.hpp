#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class MonitorConnectedEvent : public Event
    {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(MonitorConnected);
    };

    class MonitorDisconnectedEvent : public Event
    {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(MonitorDisconnected);
    };

} // namespace DampEngine