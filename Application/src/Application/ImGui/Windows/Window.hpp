#pragma once

#include "imgui.h"

class Window
{
public:
    Window(ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None)
        : m_WindowFlags(windowFlags)
    {
    }

    inline ImGuiViewport *GetMainViewport() const { return ImGui::GetMainViewport(); }
    inline void SetNextPos(float relx, float rely) { ImGui::SetNextWindowPos(ImVec2(GetMainViewport()->GetWorkPos().x + relx, GetMainViewport()->GetWorkPos().y + rely)); }
    inline void SetNextSize(float w, float h) { ImGui::SetNextWindowPos(ImVec2(w, h)); }

    virtual void Render() = 0;
    virtual ~Window() = default;
    
protected:
    ImGuiWindowFlags m_WindowFlags;
};