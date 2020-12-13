#pragma once

#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();

        virtual void OnAttach() final;
        virtual void OnUpdate() final;
        virtual void OnDetach() final;

        
        
        virtual ~ImGuiLayer() override;
    protected:
        virtual void ImGuiDescription();
    private:
        void Begin();
        void End();
    };

} // namespace DampEngine
