#include "ImGuiLayer.hpp"

#include "DampEngine/Core/Macros.hpp"
#include "DampEngine/Core/Application.hpp"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <mutex>

namespace DampEngine
{
#pragma region Layer implementations

    std::mutex ImGuiLayer::s_FrameControlMutex;
    std::mutex ImGuiLayer::s_AttachDetachMutex;

    ImGuiLayer::ImGuiLayer()
    {
    }
    
    bool ImGuiLayer::s_FrameBegun;
    int ImGuiLayer::s_InstanceCount = 0;
    void ImGuiLayer::OnAttach()
    {

        s_AttachDetachMutex.lock();
        {

            if (s_InstanceCount > 0)
            {
                s_AttachDetachMutex.unlock();
                return;
            }

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
                                                                //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
                                                                //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

            ImGui::StyleColorsDark();
            // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.

            ImGuiStyle &style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            Application &app = Application::GetCurrent();
            GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

            // Setup Platform/Renderer bindings
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 410");

            s_InstanceCount++;
        }
        s_AttachDetachMutex.unlock();
    }

    void ImGuiLayer::OnUpdate()
    {
        this->ImGuiDescription();
    }

    void ImGuiLayer::OnDetach()
    {
        s_AttachDetachMutex.lock();
        {
            DE_ASSERT(s_InstanceCount > 0, "Detaching more times than Attaching");
            s_InstanceCount--;

            if (s_InstanceCount > 0)
            {
                s_AttachDetachMutex.unlock();
                return;
            }

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        s_AttachDetachMutex.unlock();
    }

    void ImGuiLayer::BeginFrame()
    {
        s_FrameControlMutex.lock();
        {
            DE_ASSERT(!s_FrameBegun, "Wrong Begin/End frame order");
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            s_FrameBegun = true;
        }
        s_FrameControlMutex.unlock();
    }

    void ImGuiLayer::EndFrame()
    {
        s_FrameControlMutex.lock();
        {
            DE_ASSERT(s_FrameBegun, "Wrong Begin/End frame order");
            ImGuiIO &io = ImGui::GetIO();
            const Application &app = Application::GetCurrent();
            io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
            s_FrameBegun = false;
        }
        s_FrameControlMutex.unlock();
    }

    void ImGuiLayer::ImGuiDescription()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

#pragma endregion

} // namespace DampEngine
