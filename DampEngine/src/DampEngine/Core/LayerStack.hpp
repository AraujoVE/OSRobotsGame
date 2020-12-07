#pragma once

#include "Core/Layer.hpp"

#include <vector>

namespace DampEngine
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();
    private:
        std::vector<Layer*> m_LayerStack; 
    };    
} // namespace DampEngine
