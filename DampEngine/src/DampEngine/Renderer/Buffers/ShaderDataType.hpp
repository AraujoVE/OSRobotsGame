#pragma once

#include "DampEngine/Core/Macros.hpp"

namespace DampEngine
{
    enum class ShaderDataType
    {
        None = 0,
        Float1,
        Float2,
        Float3,
        Float4,
        Int1,
        Int2,
        Int3,
        Int4,
        Mat2,
        Mat3,
        Mat4,
        Bool
    };

    unsigned ShaderDataTypeCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float1:
            return 1;
        case ShaderDataType::Float2:

            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Int1:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Mat2:
            return 2 * 2;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }
        DE_ASSERT(false, "Uknown ShaderDataType!");
        return 0;
    }

    unsigned ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float1:
            return 1 * 4;
        case ShaderDataType::Float2:
            return 2 * 4;
        case ShaderDataType::Float3:
            return 3 * 4;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Int1:
            return 1 * 4;
        case ShaderDataType::Int2:
            return 2 * 4;
        case ShaderDataType::Int3:
            return 3 * 4;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Mat2:
            return 2 * 2 * 4;
        case ShaderDataType::Mat3:
            return 3 * 3 * 4;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }

        DE_ASSERT(false, "Uknown ShaderDataType!");
        return 0;
    }

} // namespace DampEngine
