#pragma once

#include "DefaultHandlers.fwd.hpp"

#include "Application/Threads/ThreadLoop.hpp"
#include "Application/Events/EventHandler/EventHandler.template.hpp"

#include "Application/Game/Types.fwd.hpp"

#include <string>

namespace Application
{
    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT(EH_TaskCreated, bool, Task &);
    DA_EVENT(EH_TaskStarted, bool, Task &);
    DA_EVENT(EH_TaskCancelled, bool, Task &);
    DA_EVENT(EH_TaskFinished, bool, Task &);
    DA_EVENT(EH_RobotsCreated, bool, int);
    DA_EVENT(EH_RobotsDestroyed, bool, int);
    DA_EVENT(EH_RobotsMoved, bool, Task &, int);

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT(EH_DecaymentStopped, bool);

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT(EH_GameStarted, bool, GameRunner &);
    DA_EVENT(EH_GameEnded, bool, GameRunner &, int);

    /* THREADLOOP EVENTS */
    DA_EVENT(EH_ThreadStarted, bool);
    DA_EVENT(EH_ThreadEnded, bool, ThreadEndedReason::ThreadEndedReason_t);

    /* EAGAME GUI LAYER EVENTS */
    DA_EVENT(EH_EAGuiPropsChanged, bool, const EvoAlg::EAGuiProps &);

    /* GAME CONSTS EVENTS */
    DA_EVENT(EH_GameConstsChanged, bool);

    /* EA HANDLERS */
    DA_EVENT(EH_EAScriptEnded, bool, int /* individualIndex */, std::vector<std::pair<double, double>> /* gameplayResults */);

} // namespace Application
