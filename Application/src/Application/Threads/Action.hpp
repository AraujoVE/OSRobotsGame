#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Events/EventListener/EventListener.hpp"

#include "ThreadLoop.hpp"

#include <memory>
#include <vector>

namespace Application
{
    template <typename... Args>
    class Action
    {
    private:
        std::function<void(Args...)> m_ActionFn;
        std::vector<ThreadLoop *> m_ThreadLoopVec;

    public:
        Action(const std::function<void(Args...)> &actionFn) : m_ActionFn(actionFn) {}
        void Invoke(Args... args)
        {
            //TODO: fix memory leak
            // const int newTLIndex = m_ThreadLoopVec.size();
            auto actionFn = m_ActionFn;
            // auto &vec = m_ThreadLoopVec;
            auto onTLEnded = [=](ThreadEndedReason::ThreadEndedReason_t _) {
                actionFn(args...);
                // delete vec[newTLIndex];
                // vec[newTLIndex] = nullptr;
                return false;
            };

            ThreadLoop *newThreadLoop = new ThreadLoop([]() {}, []() { return false; });

            newThreadLoop->m_EventListener->Register(new EH_ThreadEnded(onTLEnded));

            m_ThreadLoopVec.push_back(newThreadLoop);

            newThreadLoop->Start();
        }
    };

    class ActionVoid
    {
    private:
        std::function<void()> m_ActionFn;
        std::vector<ThreadLoop *> m_ThreadLoopVec;

    public:
        ActionVoid(const std::function<void()> &actionFn) : m_ActionFn(actionFn) {}
        void Invoke()
        {
            //TODO: fix memory leak
            // const int newTLIndex = m_ThreadLoopVec.size();
            auto actionFn = m_ActionFn;
            // auto &vec = m_ThreadLoopVec;
            auto onTLEnded = [=](ThreadEndedReason::ThreadEndedReason_t _) {
                actionFn();
                // delete vec[newTLIndex];
                // vec[newTLIndex] = nullptr;
                return false;
            };

            ThreadLoop *newThreadLoop = new ThreadLoop([]() {}, []() { return false; });

            newThreadLoop->m_EventListener->Register(new EH_ThreadEnded(onTLEnded));

            m_ThreadLoopVec.push_back(newThreadLoop);

            newThreadLoop->Start();
        }
    };
} // namespace Application
