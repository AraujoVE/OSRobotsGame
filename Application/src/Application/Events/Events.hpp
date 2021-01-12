#pragma once

#include "Application/Events/Event.hpp"

#define DA_EVENT(name, signature) class name: public EventHandler<signature> { \
    public: \
        name(std::function<signature> handler): EventHandler::EventHandler(handler, #name ) {} \
        static const std::string GetTypeStatic() { return #name; } \
    };

namespace Application
{
    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT(EH_TaskCreated, bool(Task&));
    DA_EVENT(EH_TaskEnded, bool(Task&));
    DA_EVENT(EH_RobotsCreated, bool(int));
    DA_EVENT(EH_RobotsDestroyed, bool(int));
    DA_EVENT(EH_RobotsMoved, bool(Task&,int));
    
    
} // namespace Application

