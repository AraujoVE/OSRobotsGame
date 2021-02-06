#pragma once

#include "EventListener.fwd.hpp"

#include "DampEngine/Threads/Mutex.hpp"

#include "Application/Events/EventHandler/EventHandler.template.hpp"
#include "Application/Events/EventHandler/IEventHandler.hpp"

#include "Application/Events/EventHandler/IEventHandler.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <pthread.h>

namespace Application
{
    //TODO: make some methods const
    class EventListener final
    {
        mutable DampEngine::Mutex m_MapMutex;
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
            m_MapMutex.Lock();
            for (auto &queuePair: handlerQueueMap) {
                for (IEventHandler *eHandler : queuePair.second) {
                    delete eHandler;
                }
            }
            handlerQueueMap.clear();
            m_MapMutex.Unlock();
        }
    };
} // namespace Application
