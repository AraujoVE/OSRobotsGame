#pragma once

#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate() final;
        virtual void OnDetach() override;

        
        
        ~ImGuiLayer();
    protected:
        virtual void ImGuiDescription();
    private:
        void Begin();
        void End();
    };

} // namespace DampEngine
