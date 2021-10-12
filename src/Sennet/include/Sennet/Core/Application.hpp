#pragma once

#include "Sennet/Core/Base.hpp"

#include "Sennet/Core/LayerStack.hpp"
#include "Sennet/Core/Timestep.hpp"
#include "Sennet/Core/Window.hpp"

#include "Sennet/Events/ApplicationEvent.hpp"
#include "Sennet/Events/Event.hpp"

#include "Sennet/ImGui/ImGuiLayer.hpp"

int main(int argc, char** argv);

namespace Sennet
{

class Application
{
public:
    Application(const std::string& name = "Sennet App");
    virtual ~Application();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow() { return *m_Window; }

    void Close();

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    static Application& Get() { return *s_Instance; }

private:
    void Run();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnWindowIconify(WindowIconifyEvent& e);

protected:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;

    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
    friend int ::main(int argc, char* argv[]);
};

// To be defined in client.
Application* CreateApplication();

} // namespace Sennet
