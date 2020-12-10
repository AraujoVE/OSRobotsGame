#pragma once

#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnDetach() override;
        
        //----IEventHandler callbacks----
        virtual bool OnKeyPressed(KeyPressedEvent &event) override;
        virtual bool OnKeyReleased(KeyReleasedEvent &event) override;
        virtual bool OnKeyTyped(KeyTypedEvent &event) override;

        //Mouse related
        virtual bool OnMouseEnteredWindow(MouseEnteredWindowEvent &event) override;
        virtual bool OnMouseLeftWindow(MouseLeftWindowEvent &event) override;
        virtual bool OnMouseMoved(MouseMovedEvent &event) override;

        virtual bool OnMouseButtonPressed(MouseButtonPressedEvent &event) override;
        virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent &event) override;

        virtual bool OnMouseScrolled(MouseScrolledEvent &event) override;

        //Window related
        virtual bool OnWindowClosed(WindowClosedEvent &event) override;
        virtual bool OnWindowFocused(WindowFocusedEvent &event) override;
        virtual bool OnWindowBlurred(WindowBlurredEvent &event) override;

        virtual bool OnWindowMinimized(WindowMinimizedEvent &event) override;
        virtual bool OnWindowMaximized(WindowMaximizedEvent &event) override;
        virtual bool OnWindowRestored(WindowRestoredEvent &event) override;
        virtual bool OnWindowMoved(WindowMovedEvent &event) override;
        virtual bool OnWindowRefreshed(WindowRefreshedEvent &event) override;
        virtual bool OnWindowResized(WindowResizedEvent &event) override;
        virtual bool OnWindowContentScaled(WindowContentScaledEvent &event) override;

        //Fullscreen
        virtual bool OnMonitorConnected(MonitorConnectedEvent &event) override;
        virtual bool OnMonitorDisconnected(MonitorDisconnectedEvent &event) override;

        //Joystick
        virtual bool OnJoystickConnected(JoystickConnectedEvent &event) override;
        virtual bool OnJoystickDisconnected(JoystickDisconnectedEvent &event) override;
        
        ~ImGuiLayer();
    };

} // namespace DampEngine
