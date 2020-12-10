#pragma once

#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();

        void OnAttach() override;
        void OnUpdate() override;
        void OnDetach() override;
        
        //----IEventHandler callbacks----
        bool OnKeyPressed(KeyPressedEvent &event) override;
        bool OnKeyReleased(KeyReleasedEvent &event) override;
        bool OnKeyTyped(KeyTypedEvent &event) override;

        //Mouse related
        bool OnMouseEnteredWindow(MouseEnteredWindowEvent &event) override;
        bool OnMouseLeftWindow(MouseLeftWindowEvent &event) override;
        bool OnMouseMoved(MouseMovedEvent &event) override;

        bool OnMouseButtonPressed(MouseButtonPressedEvent &event) override;
        bool OnMouseButtonReleased(MouseButtonReleasedEvent &event) override;

        bool OnMouseScrolled(MouseScrolledEvent &event) override;

        //Window related
        bool OnWindowClosed(WindowClosedEvent &event) override;
        bool OnWindowFocused(WindowFocusedEvent &event) override;
        bool OnWindowBlurred(WindowBlurredEvent &event) override;

        bool OnWindowMinimized(WindowMinimizedEvent &event) override;
        bool OnWindowMaximized(WindowMaximizedEvent &event) override;
        bool OnWindowRestored(WindowRestoredEvent &event) override;
        bool OnWindowMoved(WindowMovedEvent &event) override;
        bool OnWindowRefreshed(WindowRefreshedEvent &event) override;
        bool OnWindowResized(WindowResizedEvent &event) override;
        bool OnWindowContentScaled(WindowContentScaledEvent &event) override;

        //Fullscreen
        bool OnMonitorConnected(MonitorConnectedEvent &event) override;
        bool OnMonitorDisconnected(MonitorDisconnectedEvent &event) override;

        //Joystick
        bool OnJoystickConnected(JoystickConnectedEvent &event) override;
        bool OnJoystickDisconnected(JoystickDisconnectedEvent &event) override;
        
        ~ImGuiLayer();
    };

} // namespace DampEngine
