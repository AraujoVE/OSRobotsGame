#pragma once

#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/WindowEvents.hpp"
#include "DampEngine/Core/LayerStack.hpp"

#include <functional>

namespace DampEngine
{
    class Application : IEventHandler
    {
    protected:
        explicit Application(const WindowProps& props = WindowProps("DampApplication"));

        //--- Client defined Hooks ---
        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnStop() {}

    public:
        virtual ~Application() {}

        //Currently, the application is limited to only one window
        virtual void CreateWindow();

        //Issue the application to setup everything and start gameloop
        void Run();

        void Close();
        void OnEvent(Event &event);

        //Other event handlers can also be defined according to IEventHandler's virtual functions
        bool OnWindowClosed(WindowClosedEvent &event) override;
        bool OnWindowResized(WindowResizedEvent &event) override;

        //Application Singleton
        inline static Application& Get(); 
    protected:
        WindowProps m_WindowStartingProps;
        Window *m_Window;
        LayerStack m_LayerStack;
    private:
        static Application *s_Instance; 
        bool m_Running;
    };
} // namespace DampEngine