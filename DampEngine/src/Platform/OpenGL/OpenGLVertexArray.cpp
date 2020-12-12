#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

namespace DampEngine
{
    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_VertexArrayID);
    }

    void OpenGLVertexArray::Bind() {
        glBindVertexArray(m_VertexArrayID);
    }
    
    void OpenGLVertexArray::Unbind() {

    }
    
} // namespace DampEngine
