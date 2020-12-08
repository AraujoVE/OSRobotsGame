#include "DampEngine/Core/Assert.hpp"
#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Core/Log.hpp"

#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/KeyEvent.hpp"
#include "DampEngine/Events/MouseMovedEvent.hpp"

#include "LinuxGLFWWindow.hpp"

// #define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// #include <glad/glad.h>

#include <depch.hpp>
namespace DampEngine
{

    //Defines at "DampEngine/Core/Window.h" the Create method as being a LinuxGLFWWindow factory
    Window *Window::Create(const WindowProps &starting_props)
    {   
        DE_ENGINE_INFO("Linux System detected, creating LinuxGLFWWindow");
        return new LinuxGLFWWindow(starting_props);
    }

    //Number of windows created since the engine started,
    //the counter is NOT decremented when a window is closed.
    static unsigned s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char *description)
    {
        DE_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

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
            DE_ENGINE_TRACE("calling glfwInit()");
            int success = glfwInit();
            
            DE_ASSERT((1 == 0), "sei la")
            DE_ENGINE_TRACE("calling glfwInit()");

            // DE_ASSERT(success, "Could not initialize GLFW");
        }

        // m_GLFWWindow = glfwCreateWindow(m_Data.Props.Width, m_Data.Props.Height, m_Data.Props.Title.c_str(), nullptr, nullptr);
        // DE_ASSERT(m_GLFWWindow != nullptr, "Could not create a GLFW window");

        // s_GLFWWindowCount++;

        // glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        //     switch (action)
        //     {
        //         //GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`
        //         case GLFW_PRESS:
        //             SendEventToWindow(window, KeyPressedEvent(key, 1));
        //         break;
            
        //     default:
        //         break;
        //     }
        //     // SendEventToWindow(window, )
        // });

        // glfwSetCharCallback(m_GLFWWindow, [](GLFWwindow *window, unsigned int codepoint) {

        // });

        // // glfwSetCharModsCallback(m_GLFWWindow, [](GLFWwindow *window, unsigned int codepoint, int mods) {

        // // });

        // glfwSetCursorEnterCallback(m_GLFWWindow, [](GLFWwindow *window, int entered_or_left) {

        // });

        // glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow *window, double xpos, double ypos) {

        // });

        // // glfwSetDropCallback(m_GLFWWindow, [](GLFWwindow *window, double xpos, double ypos) {

        // // });

        // // glfwSetJoystickCallback([](int joystick_id, int event) {

        // // });

        // // glfwSetJoystickUserPointer(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // // });

        // // glfwSetMonitorCallback(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // // });

        // // glfwSetMonitorUserPointer(m_GLFWWindow, [](GLFWwindow *window, int joystick_id, int event) {

        // // });

        // glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow *window, int button, int action, int mods) {

        // });

        // glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {

        // });

        // glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow *window) {

        // });

        // // glfwSetWindowContentScaleCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {

        // // });

        // glfwSetWindowFocusCallback(m_GLFWWindow, [](GLFWwindow *window, int focus) {
        //     // 1 focused, 0 lost focus
        // });

        // glfwSetWindowIconifyCallback(m_GLFWWindow, [](GLFWwindow *window, int iconified) {
        //     // 1 iconified, 0 restored
        // });

        // glfwSetWindowMaximizeCallback(m_GLFWWindow, [](GLFWwindow *window, int maximize) {
        //     // 1 maximized, 0 restored
        // });

        // glfwSetWindowPosCallback(m_GLFWWindow, [](GLFWwindow *window, int xpos, int ypos) {

        // });

        // // glfwSetWindowRefreshCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {

        // // });

        // glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow *window, int width, int height) {

        // });
    } // LinuxGLFWWindow::InitWindowInGLFW()

} // namespace DampEngine