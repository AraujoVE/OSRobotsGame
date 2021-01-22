#pragma once

#include "EventListener.fwd.hpp"

#include "DampEngine/Threads/Mutex.hpp"

#include "Application/Events/EventHandler/EventHandler.template.hpp"

#include "Application/Game/GameRunner.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <pthread.h>

namespace Application
{
    //TODO: fix ml
    typedef std::vector<void*> HandlerQueue;
    class EventListener final
    {
        DampEngine::Mutex m_MapMutex;
        std::unordered_map<std::string, HandlerQueue> handlerQueueMap = {};

    public:
        template <class EventHandlerType>
        void On();

        template <class EventHandlerType>
        void On(typename EventHandlerType::ArgumentsTuple argTuple);

        template <class EventHandlerType>
        void OnAsync();

        template <class EventHandlerType>
        void OnAsync(typename EventHandlerType::ArgumentsTuple argTuple);

        template <class EventHandlerType>
        void Register(EventHandlerType *eventHandler);

        template <class EventHandlerType>
        void Unregister(EventHandlerType *eventHandler);

        void Clear()
        {
            m_MapMutex.Lock();
            handlerQueueMap.clear();
            m_MapMutex.Unlock();
        }
    };
} // namespace Application
