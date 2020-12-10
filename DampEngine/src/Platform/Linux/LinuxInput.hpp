#pragma once

#include "DampEngine/Core/Input.hpp"

namespace DampEngine
{
    class LinuxInput : public Input
    {
        virtual bool IsKeyPressedImpl(int keyCode) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float,float> GetMousePosImpl() override;

    };
} // namespace DampEngine