#include "ImGuiLayer.hpp"

#include "DampEngine/Core/Macros.hpp"
#include "DampEngine/Core/Application.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace DampEngine
{
#pragma region Layer implementations

    ImGuiLayer::ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        DE_ENGINE_DEBUG("ImGuiLayer::OnAttach()");

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnUpdate()
    {
        // DE_ENGINE_DEBUG("ImGuiLayer::OnUpdate()");

        ImGuiIO &io = ImGui::GetIO();
        const Application &app = Application::GetCurrent();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        static float last_time = time - (1.0f / 60.0f);
        io.DeltaTime = (time - last_time);
        last_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnDetach()
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

#pragma endregion

#pragma region IEventHandler overrides

    bool ImGuiLayer::OnKeyPressed(KeyPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        // io.MouseDown[event.getM]

        return false;
    }

    bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        // io.MouseDown[event.getM]

        return false;
    }

    bool ImGuiLayer::OnKeyTyped(KeyTypedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnMouseEnteredWindow(MouseEnteredWindowEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnMouseLeftWindow(MouseLeftWindowEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnMouseMoved(MouseMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2{event.GetXPos(), event.GetYPos()};
        return false;
    }

    bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetButton()] = true;
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.GetButton()] = false;
        return false;
    }

    bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheelH = event.GetXOffset();
        io.MouseWheel = event.GetYOffset();
        return false;
    }

    //TODO: give action or delete
    bool ImGuiLayer::OnWindowClosed(WindowClosedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowFocused(WindowFocusedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowBlurred(WindowBlurredEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowMinimized(WindowMinimizedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowMaximized(WindowMaximizedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowRestored(WindowRestoredEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowMoved(WindowMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowRefreshed(WindowRefreshedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnWindowResized(WindowResizedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2{(float)event.GetWidth(), (float)event.GetHeight()};
        //TEMPORARY
        DE_ASSERT((int)event.GetWidth() * (int)event.GetHeight() > 0, "unsigned overflow");
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }

    bool ImGuiLayer::OnWindowContentScaled(WindowContentScaledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnMonitorConnected(MonitorConnectedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnMonitorDisconnected(MonitorDisconnectedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnJoystickConnected(JoystickConnectedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

    bool ImGuiLayer::OnJoystickDisconnected(JoystickDisconnectedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        return false;
    }

#pragma endregion

} // namespace DampEngine
