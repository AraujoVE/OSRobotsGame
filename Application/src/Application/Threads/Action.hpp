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

    public:
        Action(const std::function<void(Args...)> &actionFn) : m_ActionFn(actionFn) {}
        void Invoke(Args... args)
        {
            new std::thread(m_ActionFn, args...);
        }
    };
} // namespace Application
