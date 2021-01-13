#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Application/Events/Event.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Application/header/Task.hpp"

namespace Application
{
    typedef std::unique_ptr<void*> GenericEventHandlerPtr;
    typedef std::vector<GenericEventHandlerPtr> HandlerQueue;
    class EventListener
    {
        std::unordered_map<std::string, HandlerQueue> handlerQueueMap;

    public:
        template<typename E, typename... Args>
        void On(Args... args);

        template<typename E>
        void Register(EventHandler<E> *eventHandler);
    };
} // namespace Application
