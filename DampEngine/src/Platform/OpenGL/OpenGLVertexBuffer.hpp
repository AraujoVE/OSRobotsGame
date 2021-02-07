#pragma once

#include "DampEngine/Renderer/Buffers/VertexBuffer.hpp"

namespace DampEngine
{
    class OpenGLVertexBuffer : public VertexBuffer 
    {
    public:
        OpenGLVertexBuffer(float *vertices, unsigned size);
        virtual void Bind() override;
        virtual void Unbind() override;

    private:
        unsigned m_VertexBufferID;
        unsigned m_Size;
    };
} // namespace DampEngine
