#include "ThreadLoop.hpp"

namespace Application
{
    void *threadRountine(void *threadLoopV)
    {
        auto *threadLoop = (ThreadLoop *)threadLoopV;
        threadLoop->InnerLoop();
        return NULL;
    }

    const std::function<bool()> ThreadLoop::s_DefaultAliveCheckFunction = [](){return true;};

} // namespace Application
