#pragma once

#include "DampEngine/Core/Base.hpp"
#include "DampEngine/Core/Window.hpp"

struct GLFWwindow;
namespace DampEngine
{
    class LinuxGLFWWindow : public Window
    {
    public:
        LinuxGLFWWindow(const WindowProps &starting_props)
        {
            DE_ENGINE_DEBUG("LinuxGLFWWindow constructor called");
            m_Data.Props = starting_props;
            m_Data.EventCallback = [](Event& e) {
                DE_CRITICAL("Event callback undefined, ignoring event: {0}", e);
            };

            InitWindowInGLFW();
        }

        ~LinuxGLFWWindow() { }

        virtual void OnUpdate() override;

        virtual void *GetNativeWindow() const override;

        virtual inline unsigned GetHeight() const override { return m_Data.Props.Height; }
        virtual inline unsigned GetWidth() const override { return m_Data.Props.Width; }

        virtual void SetEventCallback(const EventCallbackFn &callback) override
        {
            m_Data.EventCallback = callback;
        }

    private:
        void InitWindowInGLFW();
        friend void SendEventToWindow(GLFWwindow *&glfwWindow, Event &&event);
    private:
        struct WindowData
        {
            WindowProps Props;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
        GLFWwindow *m_GLFWWindow;
    };
} // namespace DampEngine
