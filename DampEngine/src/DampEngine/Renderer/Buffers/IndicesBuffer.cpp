#include "IndicesBuffer.hpp"

#include "DampEngine/Core/Macros.hpp"

#include "DampEngine/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLIndicesBuffer.hpp"

namespace DampEngine
{
    IndicesBuffer *IndicesBuffer::Create() {
        switch(Renderer::GetCurrentAPI()) {
            case RenderingAPI::OpenGL: return new OpenGLIndicesBuffer();
        }

        DE_ENGINE_ASSERT(false, "Unkown Renering API!");
        return 0;
    }
} // namespace DampEngine
