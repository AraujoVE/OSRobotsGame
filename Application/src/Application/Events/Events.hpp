#pragma once



    #include "Application/Events/Event.hpp"
    #include "Application/header/Task.hpp"
    #include "Application/Game/GameRunner.hpp"

    #define DA_EVENT(name, signature) \
        class name: public EventHandler<signature> { \
        public: \
            name(std::function<signature> handler): EventHandler::EventHandler(handler, #name ) {} \
            static const std::string GetTypeStatic() { return #name; } \
        };


namespace Application
{
    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT(EH_TaskCreated, bool(Task&));
    DA_EVENT(EH_TaskCancelled, bool(Task&));
    DA_EVENT(EH_TaskEnded, bool(Task&));
    DA_EVENT(EH_RobotsCreated, bool(int));
    DA_EVENT(EH_RobotsDestroyed, bool(int));
    DA_EVENT(EH_RobotsMoved, bool(Task&,int));

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT(EH_StatsDecayed, bool(void));

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT(EH_GameStarted, bool(GameRunner&));
    DA_EVENT(EH_GameEnded, bool(GameRunner&));
    
} // namespace Application

#undef DA_EVENT