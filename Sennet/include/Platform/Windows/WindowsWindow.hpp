#pragma once

#include "Sennet/Core/Window.hpp"
#include "Sennet/Renderer/GraphicsContext.hpp"

// Forward declaration.
struct GLFWwindow;

namespace Sennet
{

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProperties& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;
	
	unsigned int GetWidth() const override { return m_Data.Width; }
	unsigned int GetHeight() const override { return m_Data.Height; }

	// Window attributes.
	void SetEventCallback (const EventCallbackFn& callback) override
	{
		m_Data.EventCallback = callback;
	}
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	virtual void* GetNativeWindow() const override { return m_Window; }

private:
	virtual void Init(const WindowProperties& props);
	virtual void Shutdown();

private:
	GLFWwindow* m_Window;
	Scope<GraphicsContext> m_Context;

	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};

}
