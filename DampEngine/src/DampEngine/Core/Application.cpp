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
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
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
        DE_ENGINE_TRACE("Received event: {0}", event);
        EventDispatcher::Dispatch<WindowClosedEvent>(event, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
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