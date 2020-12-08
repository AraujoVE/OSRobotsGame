#pragma once

#include "DampEngine/Core/Base.hpp"
#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/WindowEvent.hpp"

#include <string>
#include <functional>

namespace DampEngine
{
    class Application
    {
    protected:
    public:
        explicit Application(const std::string &name = "DampApplication");
        virtual ~Application() {}

        void Run();

        void Close();
        void OnEvent(Event &event);

        virtual bool OnWindowClose(WindowClosedEvent &event);
        virtual bool OnWindowResize(WindowResizedEvent &event);

        inline static Application& Get() { DE_ASSERT(s_Instance != nullptr, "Getting non-initialized window"); return *s_Instance; }
    private:
        void InitPlatformSpecific();
    private:
        static Application *s_Instance; 

        Window *m_Window;
        std::string m_Name;
        bool m_Running;
    };
} // namespace DampEngine