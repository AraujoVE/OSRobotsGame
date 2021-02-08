#pragma once

#include "EventHandler.fwd.hpp"
#include "IEventHandler.hpp"

#include <functional>
#include <tuple>
#include <string>

namespace Application
{
    template <typename R, typename... Args>
    class EventHandler : public IEventHandler
    {
    public:
        std::function<R(Args...)> m_Handler;

        typedef R ReturnType;
        using ArgumentsTuple = std::tuple<Args...>;

        EventHandler(std::function<R(Args...)> handler, const std::string &type) : IEventHandler(type), m_Handler(handler) {}
        EventHandler(EventHandler &&) = delete;
        EventHandler(const EventHandler &) = delete;

        virtual ~EventHandler()
        {
            DE_TRACE("EventHandler ({0}) destroyed", m_Type);
        }
    };
} // namespace Application