#pragma once

#include "BufferElement.hpp"

#include <initializer_list>
#include <vector>

namespace DampEngine
{
    class BufferLayout final {
    public:
        BufferLayout(const std::initializer_list<BufferElement>& elements) 
            : m_Elements(elements)
        {
            CalculateOffsetAndStride();
        }

        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void CalculateOffsetAndStride() {
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = m_Stride; //The stride is currently the element offset
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_Elements;
        unsigned m_Stride = 0;
    };
} // namespace DampEngine
