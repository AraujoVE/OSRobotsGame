#pragma once

#include "Event.hpp"

namespace DampEngine
{
    class MouseButtonEvent : public Event
    {
    protected:
        MouseButtonEvent(int button) : m_Button(button) {}

        inline int GetButton() const { return m_Button; }

        std::string ToString() const override { return GetEventName() + " Button: " + std::to_string(m_Button); }

    protected:
        int m_Button;
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        using MouseButtonEvent::GetButton;

        DE_EVENT_SET_TYPE(MouseButtonReleased);
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        using MouseButtonEvent::GetButton;

        DE_EVENT_SET_TYPE(MouseButtonPressed);
    };

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float xPos, float yPos): m_xPos(xPos), m_yPos(yPos) {}

        std::string ToString() const override { return GetEventName() + " New position: (x=" + std::to_string(m_xPos) + ", y=" +  std::to_string(m_yPos) + ")";}

        inline float GetXPos() const { return m_xPos; } 
        inline float GetYPos() const { return m_yPos; }

        DE_EVENT_SET_TYPE(MouseMoved);
    private:
        float m_xPos, m_yPos; 
    };

    class MouseEnteredWindowEvent : public VBlankEvent
    {
    public:
        DE_EVENT_SET_TYPE(MouseEnteredWindow);
    };

    class MouseLeftWindowEvent : public VBlankEvent
    {
    public:
        DE_EVENT_SET_TYPE(MouseLeftWindow);
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset): m_xOffset(xOffset), m_yOffset(yOffset) {}

        std::string ToString() const override { return GetEventName() + " Offset: (x=" + std::to_string(m_xOffset) + ", y=" +  std::to_string(m_yOffset) + ")";}

        inline float GetXOffset() const { return m_xOffset; } 
        inline float GetYOffset() const { return m_yOffset; }

        DE_EVENT_SET_TYPE(MouseScrolled);

    private:
        float m_xOffset, m_yOffset;
    };

} // namespace DampEngine
