#pragma once

namespace Application
{
    class RobotManagementCallbacks;
    class RobotManagementCallbackProvider
    {
    public:
        virtual void FeedCallbacks(RobotManagementCallbacks callbacksObj) = 0;
    };
} // namespace Application