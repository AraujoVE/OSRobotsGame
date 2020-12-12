#pragma once

#include <string>

#include "ShaderDataType.hpp"

namespace DampEngine
{
    struct BufferElement final
    {
        ShaderDataType Type;
        std::string Name;
        unsigned Size;
        bool Normalized;
        unsigned Offset;

        BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
            : Type(type), Name(name), Size(ShaderDataTypeSize(type)), Normalized(false), Offset(0)
        {
        }

        inline unsigned GetComponentCount() const { return ShaderDataTypeCount(Type); }
    };
} // namespace DampEngine
