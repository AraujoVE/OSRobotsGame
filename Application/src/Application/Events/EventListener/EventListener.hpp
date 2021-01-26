#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <type_traits>

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
        // static_assert(std::is_base_of<decltype(EventHandler), EventHandlerType>::value, "EventHandlerType must be derived from EventHandler class");
        const std::string eventType = EventHandlerType::GetTypeStatic();

        static unsigned long count = 0;
        count++;
        m_MapMutex.Lock();

        do
        {
            const HandlerQueue &queue = handlerQueueMap[eventType];

            if (queue.empty())
            {
                break;
            }

            for (auto handlerIt = queue.begin(); handlerIt != queue.end(); handlerIt++)
            {

                void *genericEventHandler = *handlerIt;
                EventHandlerType *eventHandler = (EventHandlerType *)genericEventHandler;
                if (eventHandler->GetType() == "")
                    continue;

                std::apply(eventHandler->m_Handler, argTuple);
            }
        } while (false);

        m_MapMutex.Unlock();
    }

    template <class EventHandlerType>
    void EventListener::OnAsync()
    {
        // static_assert(std::is_base_of<EventHandler, EventHandlerType>::value, "EventHandlerType must be derived from EventHandler class");
        static_assert(std::is_same<typename EventHandlerType::ArgumentsTuple, std::tuple<>>::value, "Missing parameters");
        auto actionLambda = [=]() {
            this->On<EventHandlerType>();
        };

        Action<> action(actionLambda);
        action.Invoke();
    }

    //TODO: use default parameter = std::tuple<>({}) to avoid 2 functions
    template <class EventHandlerType>
    void EventListener::OnAsync(typename EventHandlerType::ArgumentsTuple argTuple)
    {
        // static_assert(std::is_base_of<EventHandler, EventHandlerType>::value, "EventHandlerType must be derived from EventHandler class");

        auto actionLambda = [=](typename EventHandlerType::ArgumentsTuple _argTuple) {
            this->On<EventHandlerType>(_argTuple);
        };

        Action<typename EventHandlerType::ArgumentsTuple> action(actionLambda);
        action.Invoke(argTuple);
    }
} // namespace Application
