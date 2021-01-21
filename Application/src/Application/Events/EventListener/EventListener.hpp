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

        static unsigned long count = 0;
        count++;
        m_MapMutex.Lock();

        do
        {
            // auto findIt = handlerQueueMap.find(eventType);
            // if (findIt == handlerQueueMap.end()) {
            //     handlerQueueMap[eventType] = {};
            // }

            (void) handlerQueueMap[eventType].begin();
            const HandlerQueue &queue = handlerQueueMap[eventType];
            if (queue.empty())
            {
                
                // DE_WARN("Ignoring unimplemented Event: {0}", eventType);
                break;
            }

            for (auto handlerIt = queue.begin(); !eventConsumed && handlerIt != queue.end(); handlerIt++)
            {

                void *genericEventHandler = *handlerIt;
                EventHandlerType *eventHandler = (EventHandlerType *)genericEventHandler;
                if (eventHandler->GetType() == "") continue;

                eventConsumed = std::apply(eventHandler->m_Handler, argTuple);
            }
        } while (false);

        m_MapMutex.Unlock();
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
        m_MapMutex.Lock();
        handlerQueueMap[eventType].push_back((void *)eventHandler);
        m_MapMutex.Unlock();
    }
} // namespace Application
