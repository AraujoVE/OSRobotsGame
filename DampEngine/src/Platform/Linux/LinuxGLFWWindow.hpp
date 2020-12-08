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

            InitWindowInGLFW();
        }

        ~LinuxGLFWWindow() { }

        void OnUpdate() override
        {
            DE_TRACE("Linux: OnUpdate() called");
        }

        inline unsigned GetHeight() override { return m_Data.Props.Height; }
        inline unsigned GetWidth() override { return m_Data.Props.Width; }

        void SetEventCallback(const EventCallbackFn &callback) override
        {
            m_Data.EventCallback = callback;
        }

        // void *GetNativeWindow() override {}
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