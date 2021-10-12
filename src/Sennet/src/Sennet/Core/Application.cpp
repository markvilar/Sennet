#include "Sennet/Core/Application.hpp"
#include "Sennet/Pch.hpp"

#include "GLFW/glfw3.h"

#include "Sennet/Core/Input.hpp"
#include "Sennet/Core/Log.hpp"
#include "Sennet/Core/Timestep.hpp"

#include "Sennet/Renderer/Renderer.hpp"

namespace Sennet
{

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
    SENNET_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Window::Create(WindowProperties(name));
    m_Window->SetEventCallback(SENNET_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        SENNET_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(
        SENNET_BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<WindowIconifyEvent>(
        SENNET_BIND_EVENT_FN(Application::OnWindowIconify));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
        {
            break;
        }
    }
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Close() { m_Running = false; }

void Application::Run()
{
    while (m_Running)
    {
        // Temporary.
        float time = glfwGetTime(); // Platform::GetTime
        Timestep ts = time - m_LastFrameTime;

        // Update layers.
        if (!m_Minimized)
        {
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(ts);
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
        }

        m_Window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return false;
}

bool Application::OnWindowIconify(WindowIconifyEvent& e)
{
    m_Minimized = e.IsMinimized();
    return false;
}

}; // namespace Sennet
