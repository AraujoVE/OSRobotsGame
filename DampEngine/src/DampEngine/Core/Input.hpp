#pragma once

namespace DampEngine
{
    /**
     *  This is a singleton class.
     *  It's a singleton so that Input can be subclassed to the platform respective one.
     *  Examples: WindowsInput, LinuxInput, MacInput
     */
    class Input
    {
    public:
        inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
        inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float,float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
        inline static float GetMouseX() { return GetMousePos().first; }
        inline static float GetMouseY() { return GetMousePos().second; }

    protected:

#pragma region Virtual implementations
        virtual bool IsKeyPressedImpl(int keyCode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float,float> GetMousePosImpl() = 0;
#pragma endregion

    private:
        static Input *s_Instance;
    };

} // namespace DampEngine
