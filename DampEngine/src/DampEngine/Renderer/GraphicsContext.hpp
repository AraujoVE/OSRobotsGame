#pragma once

namespace DampEngine
{
    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
} // namespace DampEngine
