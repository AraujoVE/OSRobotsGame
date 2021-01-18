#pragma once

#include "EventHandler.fwd.hpp"

#include <functional>
#include <tuple>
#include <string>

namespace Application
{
    template <typename R, typename... Args>
    class EventHandler
    {
    public:
        std::function<R(Args...)> m_Handler;

        typedef R ReturnType;
        using ArgumentsTuple = std::tuple<Args...>;

    private:
        const std::string m_Type;

    public:
        const std::string GetType() { return m_Type; }
        static const std::string GetTypeStatic() { return "EventHandler"; }
        EventHandler(std::function<R(Args...)> handler, std::string type) : m_Handler(handler), m_Type(type) {}
        EventHandler(EventHandler &&) = default;
    };

    template <typename R>
    class EventHandler<R, void>
    {
    public:
        std::function<R()> m_Handler;
        typedef R ReturnType;
        using ArgumentsTuple = std::tuple<>;

    private:
        const std::string m_Type;
        friend class Dispatcher;

    public:
        const std::string GetType() { return m_Type; }
        static const std::string GetTypeStatic() { return "EventHandler"; }
        EventHandler(std::function<R()> handler, std::string type) : m_Handler(handler), m_Type(type) {}
        EventHandler(EventHandler &&) = default;
    };
} // namespace Application