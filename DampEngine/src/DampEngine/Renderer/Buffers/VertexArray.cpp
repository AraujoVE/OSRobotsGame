#include "VertexArray.hpp"

#include "DampEngine/Core/Macros.hpp"

#include "DampEngine/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"

#define CASE_OPENGL case RenderingAPI::OpenGL

namespace DampEngine
{
    VertexArray *VertexArray::Create() {
        switch(Renderer::GetCurrentAPI()) {
            CASE_OPENGL : return new OpenGLVertexArray();
        }

        DE_ASSERT(false, "Unkown Renering API!");
        return 0;
    }

} // namespace DampEngine
