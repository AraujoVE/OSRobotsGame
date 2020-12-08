#include "Event.hpp"

#define DE_EVENT_SET_TYPE(EVTYPE) \
    inline static DampEngine::EventType GetStaticEventType() { return DampEngine::EventType::EVTYPE; }

namespace DampEngine
{

    class KeyEvent : public Event
    {
    protected:
        KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
        //GetCategory => Input, KeyEvent
        inline static EventType GetStaticEventType() = delete;

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

    private:
        unsigned int m_Repetitions;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        using KeyEvent::KeyEvent;

        DE_EVENT_SET_TYPE(KeyReleased)
    };

    class KeyTypedEvent : KeyEvent
    {
        using KeyEvent::KeyEvent;

        DE_EVENT_SET_TYPE(KeyTyped)
    };

} // namespace DampEngine