#pragma once

#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();

        void OnAttach() override;
        void OnUpdate() override;
        void OnDetach() override;
        
        //IEventHandler callbacks
        bool OnWindowResized(WindowResizedEvent &event) override;
        
        ~ImGuiLayer();
    };

} // namespace DampEngine
