#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "Application/Events/EventListener/EventListener.hpp"

#include "ThreadLoop.hpp"

#include <memory>
#include <vector>

#include <thread>

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
            std::thread(m_ActionFn, args...);
        }
    };
} // namespace Application
