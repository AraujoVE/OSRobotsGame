#pragma once

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
    class EventHandler;
    
    template <typename R>
    class EventHandler<R, void>;
} // namespace Application
