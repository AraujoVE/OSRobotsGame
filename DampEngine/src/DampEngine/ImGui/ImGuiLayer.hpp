#pragma once

#include "DampEngine/Core/Layer.hpp"
#include "DampEngine/Threads/Mutex.hpp"

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
    public:
        static void BeginFrame();
        static void EndFrame();
    private:
        //TODO: CURRENTLY SUPPORTS ONLY ONE GLFW WINDOW
        static int s_InstanceCount;
        static bool s_FrameBegun;
        static Mutex s_FrameControlMutex;
        static Mutex s_AttachDetachMutex;
    };

} // namespace DampEngine
