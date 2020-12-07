#include "Core/Assert.hpp"
#include "Core/Window.hpp"

#include <GLFW/glfw3.h>

//Number of windows created since the engine started,
//the counter is NOT decremented when a window is closed.

namespace DampEngine
{

    static unsigned s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
	{
		DE_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

    class LinuxGLFWWindow : public Window
    {
    public:
        LinuxGLFWWindow(const WindowProps &starting_props)
        {
            m_Data.WindowProps = starting_props;
            InitWindowInGLFW();
        }

        ~LinuxGLFWWindow()
        {

        }

        inline unsigned GetHeight() { return m_Data.WindowProps.Height; }
        inline unsigned GetWidth() { return m_Data.WindowProps.Width; }

        void SetEventCallback(const EventCallbackFn &callback) override
        {
            m_Data.EventCallback = callback;
        }

        // void *GetNativeWindow() override {}
    private:
        void InitWindowInGLFW();

    private:

        struct WindowData {
            WindowProps WindowProps;
            EventCallbackFn EventCallback;
        };
        
        WindowData m_Data;
        GLFWwindow *m_GLFWWindow;

    };

    Window *Window::Create(const WindowProps &starting_props)
    {
        return new LinuxGLFWWindow(starting_props);
    }

    void LinuxGLFWWindow::InitWindowInGLFW() {
        if (s_GLFWWindowCount == 0) { //Only initialize GFLW the first time
            // TODO: assert glfw is not initialized
            int success = glfwInit();
            DE_ASSERT(success, "Could not initialize GLFW");
        }

        GLFWwindow *created_window  = glfwCreateWindow(m_Data.WindowProps.Width, m_Data.WindowProps.Height, m_Data.WindowProps.Title.c_str(), nullptr, nullptr);
        DE_ASSERT(created_window != nullptr, "Could not create a GLFW window");
        m_GLFWWindow = created_window;
        
        s_GLFWWindowCount++;

        glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {

        });

        glfwSetCharCallback(m_GLFWWindow, [](GLFWwindow *window, unsigned int codepoint) {

        });

        // glfwSetCharModsCallback(m_GLFWWindow, [](GLFWwindow *window, unsigned int codepoint, int mods) {

        // });

        glfwSetCursorEnterCallback(m_GLFWWindow, [](GLFWwindow *window, int entered_or_left) {

        });

        glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow *window, double xpos, double ypos) {

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
        
        });
        
        glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
        
        });

        glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow *window) {
        
        });

        // glfwSetWindowContentScaleCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
        
        // });


        glfwSetWindowFocusCallback(m_GLFWWindow, [](GLFWwindow *window, int focus) {
            // 1 focused, 0 lost focus
        });


        glfwSetWindowIconifyCallback(m_GLFWWindow, [](GLFWwindow *window, int iconified) {
            // 1 iconified, 0 restored
        });


        glfwSetWindowMaximizeCallback(m_GLFWWindow, [](GLFWwindow *window, int maximize) {
            // 1 maximized, 0 restored
        });


        glfwSetWindowPosCallback(m_GLFWWindow, [](GLFWwindow *window, int xpos, int ypos) {
            
        });


        // glfwSetWindowRefreshCallback(m_GLFWWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
        
        // });


        glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow *window, int width, int height) {
        
        });


    }

} // namespace DampEngine