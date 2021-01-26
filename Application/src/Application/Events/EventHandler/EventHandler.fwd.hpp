#pragma once

#define DA_EVENT(name, R, ...) \
    class name: public EventHandler<R, ## __VA_ARGS__> { \
    public: \
        name(std::function< R(__VA_ARGS__) > handler): EventHandler::EventHandler(handler) {} \
        virtual const std::string GetType() override { return #name; } \
        static const std::string GetTypeStatic() { return #name; } \
    };

namespace Application
{
    template <typename R, typename... Args>
    class EventHandler;
    
    template <typename R>
    class EventHandler<R, void>;
} // namespace Application
