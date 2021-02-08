#pragma once

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
            std::thread(m_ActionFn, args...).detach();
        }
    };
} // namespace Application
