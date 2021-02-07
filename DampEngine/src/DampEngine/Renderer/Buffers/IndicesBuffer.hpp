#pragma once

namespace DampEngine
{
    class IndicesBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        static IndicesBuffer* Create();

        virtual ~IndicesBuffer() = default;
    };
} // namespace DampEngine
