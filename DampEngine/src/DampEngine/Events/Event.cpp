#include "DampEngine/Core/Base.hpp"
#include "Event.hpp"

#include "depch.hpp"
namespace DampEngine
{
    std::string Event::ToString() const {
        std::stringstream ss;
        ss << GetStaticEventName();
        //TODO: strip in release
        DE_ENGINE_WARN("class with EventType = {0} did not override properly ToString()", GetEventName());
        return ss.str();
    }
} // namespace DampEngine


std::ostream &operator<<(std::ostream &os, const DampEngine::Event &event)
{
    os << event.ToString();
    return os; //
}

std::ostream &operator<<(std::ostream &os, const DampEngine::EventType &event_type)
{
    static const std::string EventTypeStr[] = {
        
    };

    DE_ASSERT(sizeof(EventTypeStr)/sizeof(std::string) > (unsigned)event_type)
    os << EventTypeStr[(int)event_type];


    return os;
}