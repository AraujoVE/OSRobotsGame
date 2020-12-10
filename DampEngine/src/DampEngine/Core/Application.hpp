#pragma once

#include "DampEngine/Core/Window.hpp"
#include "DampEngine/Events/Event.hpp"
#include "DampEngine/Events/WindowEvents.hpp"
#include "DampEngine/Core/LayerStack.hpp"

#include <functional>

namespace DampEngine
{
    class Application : public IEventHandler
    {
    protected:
        explicit Application(const WindowProps& props = WindowProps("DampApplication"));

        //--- Client defined Hooks ---
        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnStop() {}

        //Currently, the engine is limited to only one window per application
        virtual void CreateWindow();
        virtual void InitLayers() = 0;

    public:
        virtual ~Application() {}

        //Issue the application to setup everything and start gameloop
        void Run();

        void OnEvent(Event& event);

        //Other event handlers can also be defined according to IEventHandler's virtual functions
        bool OnWindowClosed(WindowClosedEvent &event) override;
        
        void Close();

        virtual Window& GetWindow() const;

        //Application Singleton
        inline static Application &GetCurrent() { return *s_Instance; }

    protected:
        WindowProps m_WindowStartingProps;
        Window *m_Window;
        LayerStack m_LayerStack;
    private:
        static Application *s_Instance;
        bool m_Running;
    };
} // namespace DampEngine