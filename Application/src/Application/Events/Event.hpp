#pragma once

#include <functional>
#include <string>

#define DA_EVAL(x) x
#define DA_EXPAND_ARGS(...) __VA_ARGS__
#define DA_EVENT(name, R, Arglist) \
    class name: public EventHandler<R, DA_EVAL( DA_EXPAND_ARGS Arglist) > { \
    public: \
        name(std::function< R Arglist > handler): EventHandler::EventHandler(handler, #name ) {} \
        static const std::string GetTypeStatic() { return #name; } \
    };

namespace Application
{
    template <typename R, typename... Args>
    class EventHandler
    {
    public:
        std::function< R ( Args ... )> m_Handler;

        typedef R ReturnType;
        using ArgumentsTuple = std::tuple<Args...>;
    private:
        const std::string m_Type;


    public:
        const std::string GetType() { return m_Type; }
        static const std::string GetTypeStatic() { return "EventHandler"; }
        EventHandler(std::function< R ( Args ... )> handler, std::string type) : m_Handler(handler), m_Type(type) {}
        EventHandler(EventHandler&&) = default;
        friend class Dispatcher;
    };

    template <typename R>
    class EventHandler<R, void>
    {
    public:
        std::function<R ()> m_Handler;
        typedef R ReturnType;
        using ArgumentsTuple = std::tuple<>;
    private:
        const std::string m_Type;

    public:
        const std::string GetType() { return m_Type; }
        static const std::string GetTypeStatic() { return "EventHandler"; }
        EventHandler(std::function< R ()> handler, std::string type) : m_Handler(handler), m_Type(type) {}
        EventHandler(EventHandler&&) = default;
        friend class Dispatcher;
    };
} // namespace Application