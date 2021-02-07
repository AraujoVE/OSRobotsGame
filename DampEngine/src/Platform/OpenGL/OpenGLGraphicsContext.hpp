#pragma once

#include "DampEngine/Renderer/GraphicsContext.hpp"

struct GLFWwindow;
namespace DampEngine
{
    class OpenGLGraphicsContext : public GraphicsContext
    {
    public:
        OpenGLGraphicsContext(GLFWwindow *windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow *m_WindowHandle;
    };
} // namespace DampEngine
