#pragma once

#include "GLFW/glfw3.h"

#include "Sennet/Core/Window.hpp"
#include "Sennet/Renderer/GraphicsContext.hpp"

namespace Sennet
{

class LinuxWindow : public Window
{
public:
    LinuxWindow(const Window::Specification& specs);
    virtual ~LinuxWindow();

    virtual void Init() override;
    virtual void PollEvents() override;
    virtual void SwapBuffers() override;

    inline uint32_t GetWidth() const override { return m_Data.Width; }
    inline uint32_t GetHeight() const override { return m_Data.Height; }
    virtual std::pair<uint32_t, uint32_t> GetSize() const override;
    virtual std::pair<float, float> GetWindowPos() const override;

    virtual void Maximize() override;
    virtual void CenterWindow() override;

    virtual void SetEventCallback(const EventCallbackFn& callback) override;
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;
    virtual void SetResizable(bool resizable) const override;

    virtual const std::string& GetTitle() const override;
    virtual void SetTitle(const std::string& title) override;

    virtual void* GetNativeWindow() const override { return m_Window; }

private:
    virtual void Shutdown();

private:
    GLFWwindow* m_Window;
    std::unique_ptr<GraphicsContext> m_Context;
    Window::Specification m_Specification;

    struct WindowData
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

} // namespace Sennet
