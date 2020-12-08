#pragma once

#include "Log.hpp"
#include "Assert.hpp"

#define BIT(x) (1 << x)

//Used in Events to define it's type and debug string
#define DE_EVENT_SET_TYPE(type) \
        inline static EventType GetStaticEventType() { return EventType:: type; }; \
        inline static const std::string GetStaticEventName() { return #type; } \
        inline EventType GetEventType() const override { return type ## Event::GetStaticEventType(); } \
        inline std::string GetEventName() const override { return type ## Event::GetStaticEventName(); } 
