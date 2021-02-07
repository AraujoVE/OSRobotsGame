#pragma once

#include "DampEngine/Renderer/Buffers/VertexArray.hpp"

namespace DampEngine
{
    class OpenGLVertexArray : public VertexArray 
    {
    public:
        OpenGLVertexArray();
        virtual void Bind() override;
        virtual void Unbind() override;
    private:    
        unsigned m_VertexArrayID;
    };
} // namespace DampEngine
