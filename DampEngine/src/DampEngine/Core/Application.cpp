#include "DampEngine/Core/Base.hpp"

#include "DampEngine/Core/Application.hpp"

#include "depch.hpp"

#include <glad/glad.h>

namespace DampEngine
{
    Application::Application(const WindowProps& startingProps) : m_WindowStartingProps(startingProps)
    {
        DE_ENGINE_ASSERT((s_Instance == nullptr), "Application already instanced (currently only one is allowed)");
        Application::s_Instance = this;
    };

    void Application::CreateWindow() {
        DE_ENGINE_TRACE("Issuing window creation (platform-dependent)");
        m_Window = Window::Create(m_WindowStartingProps);
        DE_ENGINE_ASSERT(m_Window != nullptr, "Failed to create window");

        m_Window->SetEventCallback(DE_BIND_FN(Application::OnEvent));
    }

    void Application::Run()
    {
        DE_INFO("Starting Application");

        DE_TRACE("Calling Application::OnStart hook");
        OnStart();

        DE_TRACE("Requesting Window creation");
        CreateWindow();

        DE_TRACE("Initializing Layers");
        InitLayers();

        DE_INFO("Initialization completed successfully!");
        DE_TRACE("Entering application main loop");
        
        m_Running = true;
        while (m_Running) {
            
            glad_glClearColor(0.1f, 0.1f, 0.1f, 1.f);
            glad_glClear(GL_COLOR_BUFFER_BIT);

            // DE_TRACE("Calling Application::OnUpdate hook");
            this->OnUpdate();
            
            // DE_TRACE("Calling LayerStack::OnUpdate hook");
            m_LayerStack.OnUpdate();

            // DE_TRACE("Calling Window::OnUpdate hook");
            m_Window->OnUpdate();
        }
        DE_INFO("Stopping application...");


        DE_TRACE("Calling Application::OnStop hook");
        OnStop();

        DE_INFO("Application stopped.");
    };

    void Application::Stop()
    {
        DE_INFO("Application stop requested");
        m_Running = false;
    }

    void Application::OnEvent(Event &event)
    {
        // #define DISPATCH(event_type) DE_DISPATCH_EVENT_BIND(Application, event, event_type);
        
        // //!Important: Dispatch the current event to all possible IEventHandler virtual methods (which may be implemented by derived Applications and Layers ) 
        // MAP(DISPATCH, DE_INTERNAL_EVENT_DEFINITIONS());

        // #undef DISPATCH  
        
        IEventHandler::OnEvent(event);

        m_LayerStack.OnEvent(event);

    }   

    bool Application::OnWindowClosed(WindowClosedEvent &event)
    {
        Stop();
        return true;
    }

    inline Window& Application::GetWindow() const { 
        DE_ENGINE_ASSERT(m_Window != nullptr, "There is no Window running :(");
        return *m_Window; 
    }

    Application *Application::s_Instance = nullptr;
    
} // namespace DampEngine