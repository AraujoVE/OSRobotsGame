#pragma once

#include "imgui.h"
namespace Application
{
    class IGWindow
    {
    public:
        IGWindow(ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None)
            : m_WindowFlags(windowFlags)
        {
        }

        inline ImGuiViewport *GetMainViewport() const { return ImGui::GetMainViewport(); }
        inline void SetNextPos(float relx, float rely) { ImGui::SetNextWindowPos(ImVec2(GetMainViewport()->GetWorkPos().x + relx, GetMainViewport()->GetWorkPos().y + rely)); }
        inline void SetNextSize(float w, float h) { ImGui::SetNextWindowSize(ImVec2(w, h)); }

        virtual void Render() = 0;
        virtual ~IGWindow() = default;

    protected:
        ImGuiWindowFlags m_WindowFlags;
    };
} // namespace Application