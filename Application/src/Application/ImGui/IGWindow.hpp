#pragma once

#include "imgui.h"

namespace Application
{
    struct IGWindowProps {
        ImVec2 pos, size;
    };
    class IGWindow
    {
    public:
        IGWindow(ImGuiWindowFlags windowFlags = ImGuiBackendFlags_None)
            : m_WindowFlags(windowFlags)
        {
        }

        inline ImGuiViewport *GetMainViewport() const { return ImGui::GetMainViewport(); }
        inline void SetNextPos() { 
            ImGui::SetNextWindowPos(
                 {
                     GetMainViewport()->GetWorkPos().x + m_WindowProps.pos.x, 
                     GetMainViewport()->GetWorkPos().y + m_WindowProps.pos.y
                 }
            ); 
        }
        inline void SetNextSize() { ImGui::SetNextWindowSize(m_WindowProps.size); }

        virtual void Render() = 0;
        virtual ~IGWindow() = default;

    protected:
        IGWindowProps m_WindowProps;
        ImGuiWindowFlags m_WindowFlags;
    };
} // namespace Application