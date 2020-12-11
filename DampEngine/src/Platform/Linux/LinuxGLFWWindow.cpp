#include "DampEngine/Core/Macros.hpp"
#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Core/Log.hpp"

#include "DampEngine/Events.hpp"

#include "LinuxGLFWWindow.hpp"

#include "Platform/OpenGL/OpenGLGraphicsContext.hpp"

#include <GLFW/glfw3.h>

#include "depch.hpp"
namespace DampEngine
{
    //!IMPORTANT: Defines which class should be used for Window abstract class' static method Window::Create(...)
    Window *Window::Create(const WindowProps &starting_props)
    {   
        DE_ENGINE_INFO("Linux System chosen, creating LinuxGLFWWindow");
        return new LinuxGLFWWindow(starting_props);
    }

    //Number of windows created since the engine started,
    //the counter is NOT decremented when a window is closed.
    static unsigned s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char *description)
    {
        DE_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    //Some macros and function to avoid code repetition
    #define DE_GLFW_CASE_SEND(_case, window, event) case _case: SendEventToWindow(window, event); break;  
    #define DE_GLFW_CASE_DEFAULT(value) default: DE_ENGINE_ERROR("Unknown GLFW token: {1}", #value); break;
    inline void SendEventToWindow(GLFWwindow *&glfwWindow, Event &&event) {
        LinuxGLFWWindow::WindowData* window_data = (LinuxGLFWWindow::WindowData*) glfwGetWindowUserPointer(glfwWindow);
        window_data->EventCallback(event);
    }

    void LinuxGLFWWindow::InitWindowInGLFW()
    {
        DE_ENGINE_INFO("Initializing GLFW definitions for Linux Window");
        // //Only initialize GFLW the fi    rst time
        if (s_GLFWWindowCount == 0)
        { 
            int success = glfwInit();
            DE_ASSERT(success, "Could not initialize GLFW");
        }

        m_GLFWWindow = glfwCreateWindow(m_Data.Props.Width, m_Data.Props.Height, m_Data.Props.Title.c_str(), nullptr, nullptr);
        DE_ASSERT(m_GLFWWindow != nullptr, "Could not create a GLFW window");

        m_Context = new OpenGLGraphicsContext(m_GLFWWindow);
        m_Context->Init();


        glfwSetWindowUserPointer(m_GLFWWindow, &m_Data);
        s_GLFWWindowCount++;

        DE_ENGINE_TRACE("Setting GLFW event callbacks");

        #pragma region GLFW Callbacks

        glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            switch (action)
            {
                DE_GLFW_CASE_SEND(GLFW_PRESS, window, KeyPressedEvent(key, 0));
                DE_GLFW_CASE_SEND(GLFW_RELEASE, window, KeyReleasedEvent(key));
                DE_GLFW_CASE_SEND(GLFW_REPEAT, window, KeyPressedEvent(key, 1));
                DE_GLFW_CASE_DEFAULT(action);
            
            }
            // SendEventToWindow(window, )
        });

        // glfwSetCharModsCallback(m_GLFWWindow, [](GLFWwindow *window, unsigned int codepoint, int mods) {

        // });

        glfwSetCursorEnterCallback(m_GLFWWindow, [](GLFWwindow *window, int entered_or_left) {
            switch (entered_or_left)
            {
                DE_GLFW_CASE_SEND(GLFW_TRUE, window, MouseEnteredWindowEvent());
                DE_GLFW_CASE_SEND(GLFW_FALSE, window, MouseLeftWindowEvent());
                DE_GLFW_CASE_DEFAULT(entered_or_left);
            }
        });

        glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow *window, double xpos, double ypos) {
            SendEventToWindow(window, MouseMovedEvent(xpos, ypos));
        });

        // glfwSetDropCallback(m_GLFWWindow, [](GLFWwindow *window, double xpos, double ypos) {

        // });

        // glfwSetJoystickCallback([](int joystick_id, int event) {

        // });

        // glfwSetJoystickUserPointer(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // });


        // glfwSetMonitorCallback(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // });

        // glfwSetMonitorUserPointer(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // });

        glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow *window, int button, int action, int mods) {
            switch (action)
            {
                DE_GLFW_CASE_SEND(GLFW_PRESS, window, MouseButtonPressedEvent(button));
                DE_GLFW_CASE_SEND(GLFW_RELEASE, window, MouseButtonReleasedEvent(button));
                DE_GLFW_CASE_DEFAULT(action);
            }
        });

        glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
            SendEventToWindow(window, MouseScrolledEvent(xoffset, yoffset));
        });

        glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow *window) {
            SendEventToWindow(window, WindowClosedEvent());
        });

        glfwSetWindowContentScaleCallback(m_GLFWWindow, [](GLFWwindow *window, float xscale, float yscale) {
            SendEventToWindow(window, WindowContentScaledEvent(xscale, yscale));
        });

        glfwSetWindowFocusCallback(m_GLFWWindow, [](GLFWwindow *window, int focus_or_blur) {
            switch(focus_or_blur) {
                DE_GLFW_CASE_SEND(GLFW_TRUE, window, WindowFocusedEvent());
                DE_GLFW_CASE_SEND(GLFW_FALSE, window, WindowBlurredEvent());
                DE_GLFW_CASE_DEFAULT(focus_or_blur);
            }
        });

        glfwSetWindowIconifyCallback(m_GLFWWindow, [](GLFWwindow *window, int iconified) {
            switch(iconified) {
                DE_GLFW_CASE_SEND(GLFW_TRUE, window, WindowFocusedEvent());
                DE_GLFW_CASE_SEND(GLFW_FALSE, window, WindowRestoredEvent());
                DE_GLFW_CASE_DEFAULT(iconified);
            }
        });

        glfwSetWindowMaximizeCallback(m_GLFWWindow, [](GLFWwindow *window, int maximize) {
            switch(maximize) {
                DE_GLFW_CASE_SEND(GLFW_TRUE, window, WindowFocusedEvent());
                DE_GLFW_CASE_SEND(GLFW_FALSE, window, WindowRestoredEvent());
                DE_GLFW_CASE_DEFAULT(maximize);
            }
        });

        glfwSetWindowPosCallback(m_GLFWWindow, [](GLFWwindow *window, int xpos, int ypos) {
            SendEventToWindow(window, WindowMovedEvent(xpos, ypos));
        });

        // glfwSetWindowRefreshCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {

        // });

        glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow *window, int width, int height) {
            SendEventToWindow(window, WindowResizedEvent(width, height));
        });

        #pragma endregion
    
    } // LinuxGLFWWindow::InitWindowInGLFW()

    void LinuxGLFWWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }


} // namespace DampEngine

//FIXME: not sure if undef is needed
#undef DE_GLFW_CASE_SEND
#undef DE_GLFW_CASE_DEFAULT