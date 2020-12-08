#pragma once

#include "Event.hpp"

namespace DampEngine
{

    class WindowEvent : public Event
    {
    };

    class WindowClosedEvent : public Event
    {
    };

    class WindowResizedEvent : public Event
    {
    };

} // namespace DampEngine