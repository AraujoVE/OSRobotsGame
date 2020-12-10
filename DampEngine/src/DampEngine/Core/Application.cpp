#include "DampEngine/Core/Base.hpp"

#include "DampEngine/Core/Application.hpp"

#include "depch.hpp"

//!THINK HOW TO MAKE THIS IMPLEMENTATION DEPENDENT
#include <glad/glad.h>

namespace DampEngine
{
    Application::Application(const WindowProps& startingProps) : m_WindowStartingProps(startingProps)
    {
        DE_ENGINE_DEBUG("Application constructor called");
        DE_ASSERT((s_Instance == nullptr), "Application already instanced");
        Application::s_Instance = this;
    };

    void Application::CreateWindow() {
        DE_TRACE("Issuing window creation (platform-dependent)");
        m_Window = Window::Create(m_WindowStartingProps);
        DE_ASSERT(m_Window != nullptr, "Failed to create window");

        m_Window->SetEventCallback(DE_BIND_FN(Application::OnEvent));
    }

    void Application::Run()
    {
        OnStart();

        CreateWindow();
        InitLayers();

        while (m_Running) {
            glad_glClearColor(0x1, 0x00, 0x00, 255);
            glad_glClear(GL_COLOR_BUFFER_BIT);

            this->OnUpdate();
            
            m_LayerStack.OnUpdate();

            m_Window->OnUpdate();
        }

        OnStop();
    };

    void Application::Close()
    {
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
        Close();
        return true;
    }

    inline Window& Application::GetWindow() const { 
        DE_ASSERT(m_Window != nullptr, "There is no Window running :(");
        return *m_Window; 
    }

    Application *Application::s_Instance = nullptr;
    
} // namespace DampEngine