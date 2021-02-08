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
        DE_TRACE("MUTEX LOCK: LayerStack::~LayerStack");
        m_LayerStackMutex.lock();

        for (auto layer : m_LayerStack)
        {
            layer->OnDetach();
            delete layer;
        }

        DE_TRACE("MUTEX UNLOCK: LayerStack::~LayerStack");
        m_LayerStackMutex.unlock();
    }

    void LayerStack::OnUpdate()
    {
        if (m_LayerStack.empty()) return;

        ImGuiLayer::BeginFrame();

        for (Layer *layer : m_LayerStack)
        {
            m_LayerStackMutex.lock();

            if (layer->IsEnabled())
                layer->OnUpdate();

            m_LayerStackMutex.unlock();
        }

        ImGuiLayer::EndFrame();

    }

    void LayerStack::OnEvent(Event &event)
    {

        // DE_ENGINE_INFO("LayerStack received event {0}", event);
        for (auto layer : Util::reverse<std::vector<ImGuiLayer *>>(m_LayerStack))
        {
            if (event.IsHandled())
                break;
            layer->OnEvent(event);

        }
    }

    void LayerStack::PushOverlay(ImGuiLayer *overlay)
    {
        m_LayerStackMutex.lock();

        DE_ASSERT(overlay != nullptr);
        if (std::find(m_LayerStack.begin(), m_LayerStack.end(), overlay) != m_LayerStack.end())
        {
            overlay->Enable();
        }
        else
        {
            m_LayerStack.emplace_back(overlay);
            overlay->OnAttach();
        }

        m_LayerStackMutex.unlock();
    }

    void LayerStack::PopOverlay(ImGuiLayer *overlay)
    {

        DE_TRACE("MUTEX LOCK: LayerStack::PopOverlay");
        m_LayerStackMutex.lock();

        //TODO: find remove
        overlay->Disable();
        // overlay->OnDetach();

        DE_TRACE("MUTEX UNLOCK: LayerStack::PopOverlay");
        m_LayerStackMutex.unlock();
    }

    void LayerStack::PushLayer(ImGuiLayer *layer)
    {
        DE_DEBUG("MUTEX LOCK: LayerStack::PushLayer");
        m_LayerStackMutex.lock();

        DE_ASSERT(layer != nullptr);
        if (std::find(m_LayerStack.begin(), m_LayerStack.end(), layer) != m_LayerStack.end())
        {
            layer->Enable();
        }
        else
        {
            m_LayerStack.emplace(m_LayerStack.begin() + m_OverlayStartIndex, layer);
            m_OverlayStartIndex++;
        }

        DE_DEBUG("MUTEX UNLOCK: LayerStack::PushLayer");
        m_LayerStackMutex.unlock();
    }

    void LayerStack::PopLayer(ImGuiLayer *layer)
    {
        DE_TRACE("MUTEX LOCK: LayerStack::PopLayer");
        m_LayerStackMutex.lock();

        //TODO: find remove
        //m_OverlayStartIndex--;
        layer->Disable();
        // layer->OnDetach();

        DE_TRACE("MUTEX UNLOCK: LayerStack::PopLayer");
        m_LayerStackMutex.unlock();
    }

} // namespace DampEngine
