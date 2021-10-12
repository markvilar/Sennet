#pragma once

#include "Sennet/Core/Base.hpp"
#include "Sennet/Events/Event.hpp"

namespace Sennet
{

struct WindowProperties
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    WindowProperties(const std::string& title,
        uint32_t width = 1600,
        uint32_t height = 900)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Window
{
    // Interface representing a desktop system based window.

public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Scope<Window> Create(const WindowProperties& props);
};

} // namespace Sennet
