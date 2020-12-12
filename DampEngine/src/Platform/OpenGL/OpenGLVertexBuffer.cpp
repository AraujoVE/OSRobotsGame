#include "OpenGLVertexBuffer.hpp"

#include "DampEngine/Renderer/Buffers/BufferLayout.hpp"

#include <glad/glad.h>

namespace DampEngine
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, unsigned size) {
        glCreateBuffers(1, &m_VertexBufferID);
    }

    void OpenGLVertexBuffer::Bind() {

    }
    
    void OpenGLVertexBuffer::Unbind() {

    }
    
} // namespace DampEngine
