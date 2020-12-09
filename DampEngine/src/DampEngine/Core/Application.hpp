#pragma once

#include "DampEngine/Core/Base.hpp"
#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/WindowEvents.hpp"
#include "DampEngine/Core/LayerStack.hpp"

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

        virtual bool OnWindowClosed(WindowClosedEvent &event);
        virtual bool OnWindowResized(WindowResizedEvent &event);

        inline static Application& Get() { DE_ASSERT(s_Instance != nullptr, "Getting non-initialized window"); return *s_Instance; }
    private:
        void InitPlatformSpecific();
    private:
        static Application *s_Instance; 
        std::string m_Name;

        bool m_Running;

        Window *m_Window;
        LayerStack m_LayerStack;
    };
} // namespace DampEngine