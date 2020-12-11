#include "depch.hpp"
#include "OpenGLGraphicsContext.hpp"

#include "DampEngine/Core/Base.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace DampEngine
{
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow *windowHandle)
        :m_WindowHandle(windowHandle)
    {
        DE_ASSERT(windowHandle != nullptr, "Window handle is null");
    }

    void OpenGLGraphicsContext::Init()
    {  
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DE_ASSERT(status, "Failed to initialize Glad");
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

} // namespace DampEngine
