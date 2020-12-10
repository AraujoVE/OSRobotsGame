#include "LinuxInput.hpp"

#include "DampEngine/Core/Application.hpp"

#include <GLFW/glfw3.h>

#include "depch.hpp"
namespace DampEngine
{
    Input *Input::s_Instance = new LinuxInput();

    bool LinuxInput::IsKeyPressedImpl(int keyCode)
    {
        auto window = static_cast<GLFWwindow *>(Application::GetCurrent().GetWindow().GetNativeWindow());
        auto status = glfwGetKey(window, keyCode);

        return status == GLFW_PRESS || status == GLFW_REPEAT;
    }

    bool LinuxInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow *>(Application::GetCurrent().GetWindow().GetNativeWindow());
        auto status = glfwGetMouseButton(window, button);

        return status == GLFW_PRESS;
    }

    std::pair<float, float> LinuxInput::GetMousePosImpl() {
        auto window = static_cast<GLFWwindow *>(Application::GetCurrent().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {x, y};
    }

} // namespace DampEngine
