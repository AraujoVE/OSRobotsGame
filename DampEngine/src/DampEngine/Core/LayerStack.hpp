#pragma once

#include "DampEngine/Core/Layer.hpp"

#include <vector>

namespace DampEngine
{
    class LayerStack
    {
    public:
        LayerStack();

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);

        void PopLayer(Layer *layer);
        void PopOverlay(Layer *overlay);

        void OnUpdate();

        void OnEvent(Event &) const;

        ~LayerStack();
    private:
        std::vector<Layer*> m_LayerStack;
        std::vector<Layer*>::iterator m_OverlayStartIt;
    };    
} // namespace DampEngine
