#pragma once

namespace DampEngine
{
    class VertexBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static VertexBuffer* Create();

        virtual ~VertexBuffer() = default;
    };
} // namespace DampEngine
