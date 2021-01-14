#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "Application/Events/Event.hpp"

#include "Application/Game/GameRunner.hpp"

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
        void On() {
            static_assert(std::is_same<typename EventHandlerType::ArgumentsTuple,std::tuple<>>::value, "Missing parameters");
            On<EventHandlerType>(std::tuple<>());
        }

        template <class EventHandlerType>
        void On(typename EventHandlerType::ArgumentsTuple argTuple)
        {                
            const std::string eventType = EventHandlerType::GetTypeStatic();
            bool eventConsumed = false;

            pthread_mutex_lock(&mapMutex);
            do
            {
                const HandlerQueue &queue = handlerQueueMap[eventType];
                if (queue.empty())
                {
                    DE_WARN("Ignoring unimplemented Event: {0}", eventType);
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

        template <typename R, typename... Args>
        void Register(EventHandler<R, Args...> *eventHandler)
        {
            std::string eventType = eventHandler->GetType();
            pthread_mutex_lock(&mapMutex);
            handlerQueueMap[eventType].push_back(std::make_unique<void *>((void *)eventHandler));
            pthread_mutex_unlock(&mapMutex);
        }

        void Clear()
        {
            pthread_mutex_lock(&mapMutex);
                handlerQueueMap.clear();
            pthread_mutex_unlock(&mapMutex);
        }
    };
} // namespace Application
