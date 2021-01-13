#pragma once

#include <functional>
namespace Application
{
    template <typename signature>
    struct EventHandlerSignature;

    template <typename R, typename A>
    struct EventHandlerSignature<R(A)> {
        typedef R Return;
        typedef A Args;
    };



    template <typename T>
    class EventHandler
    {
    public:
        std::function<T> m_Handler;
    private:
        const std::string m_Type;

    public:
        const std::string& GetType() { return m_Type; }
        static const std::string GetTypeStatic() { return "EventHandler"; }
        EventHandler(std::function<T> handler, std::string type) : m_Handler(handler), m_Type(type) {}
        friend class Dispatcher;
    };

    class Dispatcher
    {
    public:
        template <typename T, typename... Args>
        static bool Dispatch(const EventHandler<T>& event, Args... args )
        {
            if (event.m_Handler != nullptr)
                event.m_Handler(args...);

            return true;
        }
    };

} // namespace Application