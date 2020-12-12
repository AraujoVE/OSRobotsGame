#pragma once

#include "DampEngine/Renderer/Buffers/IndicesBuffer.hpp"

namespace DampEngine
{
    class OpenGLIndicesBuffer : public IndicesBuffer 
    {
    public:
        OpenGLIndicesBuffer();
        virtual void Bind() override;
        virtual void Unbind() override;
    };
} // namespace DampEngine
