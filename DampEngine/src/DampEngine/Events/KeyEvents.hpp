#pragma once

#include "Event.hpp"

namespace DampEngine
{

    class KeyEvent : public Event
    {
    protected:
        KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
        
        std::string ToString() const override { return GetEventName() + " Key: " + std::to_string(m_KeyCode); };

    protected:
        int m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode, int repetitions)
            : KeyEvent(keyCode), m_Repetitions(repetitions)
        {
        }

        DE_EVENT_SET_TYPE(KeyPressed)

        inline unsigned GetRepetitions() const { return m_Repetitions; }

    private:
        const unsigned int m_Repetitions;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode): KeyEvent(keyCode) {}

        DE_EVENT_SET_TYPE(KeyReleased)
    };

    class KeyTypedEvent : KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode): KeyEvent(keyCode) {}

        DE_EVENT_SET_TYPE(KeyTyped)
    };

} // namespace DampEngine