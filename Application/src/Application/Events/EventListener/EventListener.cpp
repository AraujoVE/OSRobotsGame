#include "EventListener.template.hpp"

namespace Application
{
    void EventListener::Register(IEventHandler *eventHandler)
    {
        // static_assert(std::is_base_of<EventHandler, EventHandlerType>::value, "EventHandlerType must be derived from EventHandler class");
        std::string eventType = eventHandler->GetType();
        {
            std::lock_guard<std::mutex> mapGuard(m_MapMutex);
            handlerQueueMap[eventType].push_back(eventHandler);
        }
    }

    void EventListener::Unregister(IEventHandler *eventHandlerAddress)
    {
        // static_assert(std::is_base_of<EventHandler, EventHandlerType>::value, "EventHandlerType must be derived from EventHandler class");

        std::string eventType = eventHandlerAddress->GetType();
        {
            std::lock_guard<std::mutex> mapGuard(m_MapMutex);
            HandlerQueue &queue = handlerQueueMap[eventType];
            HandlerQueue::iterator it = std::find(queue.begin(), queue.end(), eventHandlerAddress);
            if (it == queue.end())
            {
                DE_ERROR("Trying to unregister a non-registered event of type '{0}'", eventType);
            }
            else
            {
                queue.erase(it);

                //TODO: make one line if it worked
                IEventHandler *eHandler = *it;
                delete eHandler;
            }
        }
    }
} // namespace Application