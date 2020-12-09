#include "Application.hpp"

#include "depch.hpp"
namespace DampEngine
{
    Application::Application(const std::string &name) : m_Name(name)
    {
        DE_ENGINE_DEBUG("Application constructor called");
        DE_ASSERT((s_Instance == nullptr), "Application already instanced");
        Application::s_Instance = this;

        DE_ENGINE_TRACE("Initializing application according to platform");
        InitPlatformSpecific();
        m_Window->SetEventCallback(DE_BIND_FN(Application::OnEvent));
    };

    void Application::Run()
    {
        while (m_Running) {
            m_Window->OnUpdate();
        }
    };

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event &event)
    {
        DE_ENGINE_TRACE("Application received event: {0}", event);
        DE_DISPATCH_EVENT_BIND(Application, event, WindowClosed);
        DE_DISPATCH_EVENT_BIND(Application, event, WindowResized);

        m_LayerStack.OnEvent(event);
    }   

    bool Application::OnWindowClosed(WindowClosedEvent &event)
    {
        Close();
        return true;
    }
    bool Application::OnWindowResized(WindowResizedEvent &event)
    {
        return true;
    }

    void Application::InitPlatformSpecific()
    {
        DE_ENGINE_DEBUG("Inside InitPlatformSpecific");
        m_Window = Window::Create(WindowProps());
    }
    
    Application *Application::s_Instance = nullptr;
    
} // namespace DampEngine