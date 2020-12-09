#pragma once

#include "DampEngine/Events/Event.hpp"

#include <string>
#include <functional>

namespace DampEngine
{
    struct WindowProps
    {
        std::string Title;
        unsigned Width, Height;
        WindowProps(const std::string &title = "DampEngine Window",
                    unsigned width = 1600,
                    unsigned height = 900)
            : Title(title), Width(width), Height(height) {}
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event &)>;

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned GetWidth() = 0;
        virtual unsigned GetHeight() = 0;

        virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

        // // Window attributes
        // virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        // virtual void SetVSync(bool enabled) = 0;
        // virtual bool IsVSync() const = 0;

        // virtual void *GetNativeWindow() = 0;

        //To be defined by Platform
        static Window *Create(const WindowProps &starting_props);
    };

} // namespace DampEngine