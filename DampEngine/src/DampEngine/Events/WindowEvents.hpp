#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class WindowClosedEvent : public Event
    {
    public:
        using Event::Event;

        virtual std::string ToString() const override {
            return GetEventName();
        }

        DE_EVENT_SET_TYPE(WindowClosed);
    };

    class WindowResizedEvent : public Event
    {
    public:
        WindowResizedEvent(unsigned width, unsigned height): m_Width(width), m_Height(height) {}
        DE_EVENT_SET_TYPE(WindowResized);

        inline unsigned GetWidth() const { return m_Width; }
        inline unsigned GetHeight() const { return m_Height; }

    private:
        unsigned m_Width, m_Height;

    };


    class WindowFocusedEvent : public VBlankEvent {
    public:
        DE_EVENT_SET_TYPE(WindowFocused);
    };

    class WindowBlurredEvent : public VBlankEvent {
    public:
        DE_EVENT_SET_TYPE(WindowBlurred);
    };

    class WindowMinimizedEvent : public VBlankEvent {
    public:
        DE_EVENT_SET_TYPE(WindowMinimized);
    };

    class WindowMaximizedEvent : public VBlankEvent {
    public:
        DE_EVENT_SET_TYPE(WindowMaximized);
    };

    class WindowRestoredEvent : public VBlankEvent {
    public:
        DE_EVENT_SET_TYPE(WindowRestored);
    };

    class WindowMovedEvent : public Event{
    public:
        WindowMovedEvent(int xPos, int yPos): m_xPos(xPos), m_yPos(yPos) {}
        DE_EVENT_SET_TYPE(WindowMoved);
    private:
        int m_xPos, m_yPos;
    };

    class WindowRefreshedEvent : public Event {
    public:
        using Event::Event;
        DE_EVENT_SET_TYPE(WindowRefreshed);
    };

    class WindowContentScaledEvent : public Event {
    public:
        WindowContentScaledEvent(float xScale, float yScale): m_xScale(xScale), m_yScale(yScale) {}

        virtual std::string ToString() const override { return GetEventName() + " Scale: (x=" + std::to_string(m_xScale) + ", y=" +  std::to_string(m_yScale) + ")";}

        DE_EVENT_SET_TYPE(WindowContentScaled);

    private:
        float m_xScale, m_yScale;
    };

} // namespace DampEngine