#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "Application/Events/EventListener/EventListener.template.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Application/Threads/Action.hpp"

#include <pthread.h>

namespace Application
{
    template <class EventHandlerType>
    void EventListener::On()
    {
        static_assert(std::is_same<typename EventHandlerType::ArgumentsTuple, std::tuple<>>::value, "Missing parameters");
        On<EventHandlerType>(std::tuple<>());
    }

    template <class EventHandlerType>
    void EventListener::On(typename EventHandlerType::ArgumentsTuple argTuple)
    {
        const std::string eventType = EventHandlerType::GetTypeStatic();
        bool eventConsumed = false;

        pthread_mutex_lock(&mapMutex);
        do
        {
            const HandlerQueue &queue = handlerQueueMap[eventType];
            if (queue.empty())
            {
                // DE_WARN("Ignoring unimplemented Event: {0}", eventType);
                break;
            }

            for (auto handlerIt = queue.begin(); !eventConsumed && handlerIt != queue.end(); handlerIt++)
            {

                void *genericEventHandler = *handlerIt->get();
                EventHandlerType *eventHandler = (EventHandlerType *)genericEventHandler;

                eventConsumed = std::apply(eventHandler->m_Handler, argTuple);
            }
        } while (false);

        pthread_mutex_unlock(&mapMutex);
    }

    template <class EventHandlerType>
    void EventListener::OnAsync()
    {
        static_assert(std::is_same<typename EventHandlerType::ArgumentsTuple, std::tuple<>>::value, "Missing parameters");
        auto actionLambda = [=]() {
            this->On<EventHandlerType>();
        };

        ActionVoid action(actionLambda);
        action.Invoke();
    }

    template <class EventHandlerType>
    void EventListener::OnAsync(typename EventHandlerType::ArgumentsTuple argTuple)
    {

        auto actionLambda = [=](typename EventHandlerType::ArgumentsTuple _argTuple) {
            this->On<EventHandlerType>(_argTuple);
        };

        Action<typename EventHandlerType::ArgumentsTuple> action(actionLambda);
        action.Invoke(argTuple);
    }

    template <class EventHandlerType>
    void EventListener::Register(EventHandlerType *eventHandler)
    {
        std::string eventType = eventHandler->GetType();
        pthread_mutex_lock(&mapMutex);
        handlerQueueMap[eventType].push_back(std::make_unique<void *>((void *)eventHandler));
        pthread_mutex_unlock(&mapMutex);
    }
} // namespace Application
