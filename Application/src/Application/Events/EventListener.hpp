#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
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

        template <typename E, typename... Args>
        void On(Args... args)
        {
            const std::string eventType = E::GetTypeStatic();
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
                    E *eventHandler = (E *)genericEventHandler;
                    eventConsumed = eventHandler->m_Handler(args...);

                    //TODO: remove need for typename Args
                    // Dispatcher::Dispatch<E, Args...>(eventHandler, args...);
                }
            } while (false);

            pthread_mutex_unlock(&mapMutex);
        }

        template <typename E>
        void Register(EventHandler<E> *eventHandler)
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
