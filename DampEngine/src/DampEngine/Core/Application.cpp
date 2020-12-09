#include "DampEngine/Core/Base.hpp"

#include "DampEngine/Core/Application.hpp"

#include "depch.hpp"
namespace DampEngine
{
    Application::Application(const std::string &name) : m_Name(name)
    {
        DE_ENGINE_DEBUG("Application constructor called");
        DE_ASSERT((s_Instance == nullptr), "Application already instanced");
        Application::s_Instance = this;
    };

    void Application::CreateWindow() {
        DE_TRACE("Issuing window creation (platform-dependent)");
        m_Window = Window::Create(WindowProps());
        DE_ASSERT(m_Window != nullptr, "Failed to create window");

        m_Window->SetEventCallback(DE_BIND_FN(Application::OnEvent));
    }

    void Application::Run()
    {
        CreateWindow();

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
    
    inline Application& Application::Get() {
        DE_ASSERT(s_Instance != nullptr, "Getting non-initialized window");
        return *s_Instance;
    }

    Application *Application::s_Instance = nullptr;
    
} // namespace DampEngine