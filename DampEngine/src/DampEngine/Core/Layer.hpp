#pragma once

#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/IEventHandler.hpp"

namespace DampEngine
{
    class Layer : virtual IEventHandler
    {
    public:

        friend class LayerStack;
        Layer(bool enabled = true): m_Enabled(enabled) {}

        virtual void OnAttach() {}
        virtual void OnUpdate() = 0;
        virtual void OnDetach() {}

        inline bool IsEnabled() { return m_Enabled; }

        virtual ~Layer() = default;
    private:
        inline void Enable() { m_Enabled = true; }
        inline void Disable() { m_Enabled = false; }
        bool m_Enabled;
    };
    
} // namespace DampEngine
