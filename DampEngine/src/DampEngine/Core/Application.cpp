#include "Application.hpp"

namespace DampEngine
{
    Application::Application(const std::string &name) : m_Name(name)
    {
        DE_ENGINE_DEBUG("Application constructor called");
        DE_ASSERT((s_Instance == nullptr), "Application already instanced");
        Application::s_Instance = this;

        DE_ENGINE_TRACE("Initializing application according to platform");
        InitPlatformSpecific();
    };

    void Application::Run()
    {
        while (m_Running)
            continue;
    };

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event &event)
    {
        //FIXME: AAAAAA
        // EventDispatcher::Dispatch<WindowClosedEvent>(event, [this](auto&&... args) -> decltype(auto) { return this->OnWindowClose(std::forward<decltype(args)>(args)...););
    }

    bool Application::OnWindowClose(WindowClosedEvent &event)
    {
        Close();
        return true;
    }
    bool Application::OnWindowResize(WindowResizedEvent &event)
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