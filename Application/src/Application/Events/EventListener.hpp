#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Application/Events/Events.hpp"

namespace Application
{
    typedef std::unique_ptr<void*> GenericEventHandlerPtr;
    typedef std::vector<GenericEventHandlerPtr> HandlerQueue;
    class EventListener
    {
        std::unordered_map<std::string, HandlerQueue> handlerQueueMap;

    public:
        template<typename E, typename... Args>
        void On(Args... args) {
            const std::string eventType = E::GetTypeStatic();
            std::unordered_map<std::string, Application::HandlerQueue>::const_iterator mapIt = handlerQueueMap.find(eventType);
            if (mapIt == handlerQueueMap.end()) return; // No handlers registered

            const HandlerQueue& queue = mapIt->second;
            if (queue.empty()) return;


            bool eventConsumed = false;

            for (auto handlerIt = queue.begin(); !eventConsumed && handlerIt != queue.end(); handlerIt++) {

                void *genericEventHandler = *handlerIt->get();
                E* eventHandler = (E*)genericEventHandler;
                eventConsumed = eventHandler->m_Handler(args...);
                
                // Dispatcher::Dispatch<E, Args...>(eventHandler, args...);

            }

            if (!eventConsumed)
                DE_WARN("Ignoring unconsumed event @EventListener.On(): {0}", eventType);
            
        }

        template<typename E>
        void Register(EventHandler<E> *eventHandler) {
            handlerQueueMap[eventHandler->GetType()].push_back(std::make_unique<void*>((void*)eventHandler));
        }

        ~EventListener() {
            //TODO: free all EventHandlers
        }
    };
} // namespace Application
