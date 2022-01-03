#pragma once

#include "Sennet/Core/Base.hpp"
#include "Sennet/Events/Event.hpp"

namespace Sennet
{

class Window
{
    // Interface representing a desktop system based window.

public:
    struct Specification
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool Fullscreen;
        bool VSync;
    };

public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;

    virtual void Init() = 0;
    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
    virtual std::pair<float, float> GetWindowPos() const = 0;

    virtual void Maximize() = 0;
    virtual void CenterWindow() = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;
    virtual void SetResizable(bool resizable) const = 0;

    virtual const std::string& GetTitle() const = 0;
    virtual void SetTitle(const std::string& title) = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const Specification& specs);
};

} // namespace Sennet
