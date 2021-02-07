#pragma once


namespace DampEngine
{
    class VertexArray
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static VertexArray* Create();

        virtual ~VertexArray() = default;
    };
} // namespace DampEngine
