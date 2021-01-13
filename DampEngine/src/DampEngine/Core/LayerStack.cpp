#include "DampEngine/Core/Base.hpp"

#include "LayerStack.hpp"
#include "DampEngine/Events.hpp"

namespace DampEngine
{

    namespace Util
    {
        template <typename T>
        class reverse
        {
        private:
            const T &m_Iterable;

        public:
            explicit reverse(const T &iterable) : m_Iterable(iterable) {}
            auto begin() { return std::rbegin(m_Iterable); }
            auto end() { return std::rend(m_Iterable); }
        };
    } // namespace Util

    LayerStack::LayerStack() {}

    LayerStack::~LayerStack()
    {
        for (auto layer : m_LayerStack)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::OnUpdate()
    {
        for (Layer *layer : m_LayerStack)
        {
            if (layer->IsEnabled())
                layer->OnUpdate();
        }
    }

    void LayerStack::OnEvent(Event &event) const
    {
        // DE_ENGINE_INFO("LayerStack received event {0}", event);
        for (auto layer : Util::reverse<std::vector<Layer *>>(m_LayerStack))
        {
            if (event.IsHandled())
                break;
            layer->OnEvent(event);
        }
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_LayerStack.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopOverlay(Layer *overlay)
    {
        //TODO: find remove
        overlay->Disable();
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        m_LayerStack.emplace(m_LayerStack.begin() + m_OverlayStartIndex, layer);
        m_OverlayStartIndex++;
        layer->OnAttach();
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        //TODO: find remove
        //m_OverlayStartIndex--;
        layer->Disable();
    }

} // namespace DampEngine
