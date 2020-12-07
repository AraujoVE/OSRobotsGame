#pragma once

#include <string>

namespace DampEngine
{
    class Application
    {
    protected:
        explicit Application(const std::string &name = "DampApplication") : m_Name(name) {
            
        }
    public:
        virtual ~Application() {}

        virtual void Run() {};

        inline static Application& Get() { return *s_Instance; }
    private:
        static Application *s_Instance; 
        std::string m_Name;
    };

} // namespace DampEngine