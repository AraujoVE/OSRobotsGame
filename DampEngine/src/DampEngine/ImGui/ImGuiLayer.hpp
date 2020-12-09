#pragma once

#include "DampEngine/Core/Log.hpp"
#include "DampEngine/Core/Layer.hpp"

namespace DampEngine
{
    class ImGuiLayer : public Layer
    {
        void OnUpdate() override {
            DE_ENGINE_TRACE("ImGui OnUpdate()");
        }
    };

} // namespace DampEngine
