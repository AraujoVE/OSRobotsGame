#include "VertexBuffer.hpp"

#include "DampEngine/Core/Macros.hpp"

#include "DampEngine/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexBuffer.hpp"

namespace DampEngine
{
    VertexBuffer *VertexBuffer::Create() {
        switch(Renderer::GetCurrentAPI()) {
            // case RenderingAPI::OpenGL: return new OpenGLVertexBuffer();
        }

        DE_ASSERT(false, "Unkown Renering API!");
        return nullptr;
    }
} // namespace DampEngine
