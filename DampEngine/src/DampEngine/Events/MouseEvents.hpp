#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class MouseButtonEvent : public Event
    {
    protected:
        MouseButtonEvent(int button) : m_Button(button) {}

    protected:
        int m_Button;
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        using MouseButtonEvent::MouseButtonEvent;
        DE_EVENT_SET_TYPE(MouseButtonReleased);
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        using MouseButtonEvent::MouseButtonEvent;
        DE_EVENT_SET_TYPE(MouseButtonPressed);
    };

    class MouseMovedEvent : Event
    {
    public:
        MouseMovedEvent() {}

        DE_EVENT_SET_TYPE(MouseMoved);
    };

    class MouseEnteredWindowEvent : Event
    {
    public:
        MouseEnteredWindowEvent() {}

        DE_EVENT_SET_TYPE(MouseEnteredWindow);
    };

    class MouseLeftWindowEvent : Event
    {
    public:
        MouseLeftWindowEvent() {}

        DE_EVENT_SET_TYPE(MouseLeftWindow);
    };

    class MouseScrolledEvent : Event
    {
    public:
        MouseScrolledEvent() {}

        DE_EVENT_SET_TYPE(MouseScrolled);
    };

} // namespace DampEngine
