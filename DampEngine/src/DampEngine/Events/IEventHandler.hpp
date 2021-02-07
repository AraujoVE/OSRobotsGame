#pragma once
#include "DampEngine/Events.hpp"

namespace DampEngine
{
    class IEventHandler
    {
    public:
        //Key related
        virtual bool OnKeyPressed(KeyPressedEvent &event);
        virtual bool OnKeyReleased(KeyReleasedEvent &event);
        virtual bool OnKeyTyped(KeyTypedEvent &event);

        //Mouse related
        virtual bool OnMouseEnteredWindow(MouseEnteredWindowEvent &event);
        virtual bool OnMouseLeftWindow(MouseLeftWindowEvent &event);
        virtual bool OnMouseMoved(MouseMovedEvent &event);

        virtual bool OnMouseButtonPressed(MouseButtonPressedEvent &event);
        virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent &event);

        virtual bool OnMouseScrolled(MouseScrolledEvent &event);

        //Window related
        virtual bool OnWindowClosed(WindowClosedEvent &event);
        virtual bool OnWindowFocused(WindowFocusedEvent &event);
        virtual bool OnWindowBlurred(WindowBlurredEvent &event);

        virtual bool OnWindowMinimized(WindowMinimizedEvent &event);
        virtual bool OnWindowMaximized(WindowMaximizedEvent &event);
        virtual bool OnWindowRestored(WindowRestoredEvent &event);
        virtual bool OnWindowMoved(WindowMovedEvent &event);
        virtual bool OnWindowRefreshed(WindowRefreshedEvent &event);
        virtual bool OnWindowResized(WindowResizedEvent &event);
        virtual bool OnWindowContentScaled(WindowContentScaledEvent &event);

        //Fullscreen
        virtual bool OnMonitorConnected(MonitorConnectedEvent &event);
        virtual bool OnMonitorDisconnected(MonitorDisconnectedEvent &event);

        //Joystick
        virtual bool OnJoystickConnected(JoystickConnectedEvent &event);
        virtual bool OnJoystickDisconnected(JoystickDisconnectedEvent &event);

        //Generic event received and passed to the other functions
        void OnEvent(Event& event);
    };

} // namespace DampEngine



