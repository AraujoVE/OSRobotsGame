#pragma once

#include "EventListener.fwd.hpp"

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
    typedef std::unique_ptr<void *> GenericEventHandlerPtr;
    typedef std::vector<GenericEventHandlerPtr> HandlerQueue;
    class EventListener
    {
        pthread_mutex_t mapMutex;
        std::unordered_map<std::string, HandlerQueue> handlerQueueMap;

    public:
        EventListener()
        {
            pthread_mutex_init(&mapMutex, NULL);
        }

        ~EventListener()
        {
            pthread_mutex_destroy(&mapMutex);
        }

        template <class EventHandlerType>
        void On();

        template <class EventHandlerType>
        void On(typename EventHandlerType::ArgumentsTuple argTuple);

        template <class EventHandlerType>
        void OnAsync();

        template <class EventHandlerType>
        void OnAsync(typename EventHandlerType::ArgumentsTuple argTuple);
        
        template <typename R, typename... Args>
        void Register(EventHandler<R, Args...> *eventHandler);

        void Clear()
        {
            pthread_mutex_lock(&mapMutex);
            handlerQueueMap.clear();
            pthread_mutex_unlock(&mapMutex);
        }
    };
} // namespace Application
