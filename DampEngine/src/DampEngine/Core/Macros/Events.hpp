#pragma once

#include "DampEngine/Events/Event.hpp"


//Used in Events to define it's type and debug string
#define DE_EVENT_SET_TYPE(type) \
    inline static EventType GetStaticEventType() { return EventType:: type; }; \
    inline static const std::string GetStaticEventName() { return #type; } \
    inline virtual EventType GetEventType() const override { return type ## Event::GetStaticEventType(); } \
    inline virtual std::string GetEventName() const override { return type ## Event::GetStaticEventName(); } 



#define DE_EVENT_GET_CLASS(event_type) PRIMITIVE_CAT(event_type, Event)
#define DE_EVENT_GET_DISPATCH_FUNCTION(event_type) PRIMITIVE_CAT(On, event_type)

//As the function names are always On ## EVENTTYPE
#define DE_DISPATCH_EVENT_BIND(class,event,event_type) EventDispatcher::Dispatch<DE_EVENT_GET_CLASS(event_type)>(    \
                                                        event,                                                 \
                                                        DE_BIND_FN(class::DE_EVENT_GET_DISPATCH_FUNCTION(event_type)) \
                                                 ) 

