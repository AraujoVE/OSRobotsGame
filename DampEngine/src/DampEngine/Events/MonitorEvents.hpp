#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class MonitorConnectedEvent : Event
    {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(MonitorConnected);
    };

    class MonitorDisconnectedEvent : Event
    {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(MonitorDisconnected);
    };

} // namespace DampEngine