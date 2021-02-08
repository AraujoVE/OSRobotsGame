#pragma once

#include "EventListener.fwd.hpp"


#include "Application/Events/EventHandler/EventHandler.template.hpp"
#include "Application/Events/EventHandler/IEventHandler.hpp"

#include "Application/Events/EventHandler/IEventHandler.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <mutex>

namespace Application
{
    //TODO: make some methods const
    class EventListener final
    {
        mutable std::mutex m_MapMutex;
        using HandlerQueue = std::vector<IEventHandler*>;
        std::unordered_map<std::string, HandlerQueue> handlerQueueMap = {};

    public:
        ~EventListener() { Clear(); }

        template <class EventHandlerType>
        void On();

        template <class EventHandlerType>
        void On(typename EventHandlerType::ArgumentsTuple argTuple);

        template <class EventHandlerType>
        void OnAsync();

        template <class EventHandlerType>
        void OnAsync(typename EventHandlerType::ArgumentsTuple argTuple);

        void Register(IEventHandler *eventHandler);

        void Unregister(IEventHandler *eventHandler);

        void Clear()
        {
            DE_DEBUG("Mutex lock: EventListener::Clear()");
            m_MapMutex.lock();
            for (auto &queuePair: handlerQueueMap) {
                for (IEventHandler *eHandler : queuePair.second) {
                    delete eHandler;
                }
            }
            handlerQueueMap.clear();
            m_MapMutex.unlock();
            DE_DEBUG("Mutex unlock: EventListener::Clear()");
        }
    };
} // namespace Application
