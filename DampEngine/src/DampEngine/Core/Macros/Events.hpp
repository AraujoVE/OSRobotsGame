#pragma once

//Used in Events to define it's type and debug string
#define DE_EVENT_SET_TYPE(type) \
        inline static EventType GetStaticEventType() { return EventType:: type; }; \
        inline static const std::string GetStaticEventName() { return #type; } \
        inline EventType GetEventType() const override { return type ## Event::GetStaticEventType(); } \
        inline std::string GetEventName() const override { return type ## Event::GetStaticEventName(); } 



#define DE_EVENT_GET_CLASS(event_type) CAT(event_type, Event)
#define DE_EVENT_GET_DISPATCH_FUNCTION(event_type) CAT(On, event_type)

//As the function names are always On ## EVENTTYPE
#define DE_DISPATCH_EVENT_BIND(class,event,event_type) EventDispatcher::Dispatch<DE_EVENT_GET_CLASS(event_type)>(    \
                                                        event,                                                 \
                                                        DE_BIND_FN(class::DE_EVENT_GET_DISPATCH_FUNCTION(event_type)) \
                                                 ) 

