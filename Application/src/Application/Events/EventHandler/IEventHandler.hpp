#pragma once

#include "DampEngine/Core/Macros/Log.hpp"
#include <string>

namespace Application
{
    class IEventHandler
    {
        private:
        std::string m_Type = "IEventHandler (this should not happen, oops...)";
    public:
        IEventHandler(const std::string& type) : m_Type(type) {}
        virtual const std::string GetType() = 0;
        static const std::string GetTypeStatic()
        {
            DE_CRITICAL("(IEventHandler) Some event forgot to override 'static GetTypeStatic()' ");
            return "UNINPLEMENTED";
        }
        virtual ~IEventHandler() = default;
    };
} // namespace Application