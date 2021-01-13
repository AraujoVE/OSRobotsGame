#include "Application/Events/EventListener.hpp"

namespace Application
{
    template <typename E, typename... Args>
    void EventListener::On(Args... args)
    {
        const std::string eventType = E::GetTypeStatic();
        std::unordered_map<std::string, Application::HandlerQueue>::const_iterator mapIt = handlerQueueMap.find(eventType);
        if (mapIt == handlerQueueMap.end())
            return; // No handlers registered

        const HandlerQueue &queue = mapIt->second;
        if (queue.empty())
            return;

        bool eventConsumed = false;

        for (auto handlerIt = queue.begin(); !eventConsumed && handlerIt != queue.end(); handlerIt++)
        {

            void *genericEventHandler = *handlerIt->get();
            E *eventHandler = (E *)genericEventHandler;
            eventConsumed = eventHandler->m_Handler(args...);


            //TODO: remove need for typename Args 
            // Dispatcher::Dispatch<E, Args...>(eventHandler, args...);
        }

        if (!eventConsumed)
            DE_WARN("Ignoring unconsumed event @EventListener.On(): {0}", eventType);
    }

    template <typename E>
    void EventListener::Register(EventHandler<E> *eventHandler)
    {
        handlerQueueMap[eventHandler->GetType()].push_back(std::make_unique<void *>((void *)eventHandler));
    }
} // namespace Application
