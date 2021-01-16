#pragma once

#include "DampEngine/ImGui/ImGuiLayer.hpp"
#include "DampEngine/Threads/Mutex.hpp"

#include <vector>

namespace DampEngine
{
    class LayerStack
    {
    public:
        LayerStack();

        void PushLayer(ImGuiLayer *layer);
        void PushOverlay(ImGuiLayer *overlay);

        void PopLayer(ImGuiLayer *layer);
        void PopOverlay(ImGuiLayer *overlay);

        void OnUpdate();

        void OnEvent(Event &);

        ~LayerStack();

    private:
        std::vector<ImGuiLayer *> m_LayerStack;
        unsigned int m_OverlayStartIndex = 0;

        //Avoid layer changes while rendering
        Mutex m_LayerStackMutex;
    };
} // namespace DampEngine
