#include "Pine/Core/Application.hpp"

#include <GLFW/glfw3.h>

#include "Pine/Core/Input.hpp"
#include "Pine/Core/Log.hpp"
#include "Pine/Core/Timestep.hpp"
#include "Pine/Pch.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
{

Application* Application::s_Instance = nullptr;

Application::Application(const Application::Specification& specs)
    : m_Specification(specs)
{
    PINE_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    Window::Specification windowSpecs;
    windowSpecs.Title = m_Specification.Name;
    windowSpecs.Width = m_Specification.WindowWidth;
    windowSpecs.Height = m_Specification.WindowHeight;
    windowSpecs.Fullscreen = m_Specification.Fullscreen;
    windowSpecs.VSync = m_Specification.VSync;

    m_Window = std::unique_ptr<Window>(Window::Create(windowSpecs));
    m_Window->Init();
    m_Window->SetEventCallback(PINE_BIND_EVENT_FN(Application::OnEvent));

    if (m_Specification.StartMaximized)
    {
        m_Window->Maximize();
    }
    else
    {
        m_Window->CenterWindow();
    }

    m_Window->SetResizable(m_Specification.Resizable);
    m_Window->SetVSync(m_Specification.VSync);

    Renderer::Init();

    if (m_Specification.EnableImGui)
    {
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
}

Application::~Application()
{
    m_Window->SetEventCallback([](Event& e) {});

    for (Layer* layer : m_LayerStack)
    {
        layer->OnDetach();
        delete layer;
    }
}

void Application::Run()
{
    OnInit();
    while (m_Running)
    {
        // TODO: Temporary.
        auto time = glfwGetTime(); // Platform::GetTime
        m_Timestep = time - m_LastFrameTime;

        m_Window->PollEvents();

        // Update layers.
        if (!m_Minimized)
        {
            m_LastFrameTime = time;
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(m_Timestep);
            }

            if (m_Specification.EnableImGui)
            {
                RenderImGui();
            }

            m_Window->SwapBuffers();
        }
    }
    OnShutdown();
}

void Application::Close() { m_Running = false; }

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        PINE_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(
        PINE_BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<WindowIconifyEvent>(
        PINE_BIND_EVENT_FN(Application::OnWindowIconify));

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

void Application::PopLayer(Layer* layer)
{
    m_LayerStack.PopLayer(layer);
    layer->OnDetach();
}

void Application::PopOverlay(Layer* layer)
{
    m_LayerStack.PopOverlay(layer);
    layer->OnDetach();
}

void Application::RenderImGui()
{
    m_ImGuiLayer->Begin();
    for (Layer* layer : m_LayerStack)
    {
        layer->OnImGuiRender();
    }
    m_ImGuiLayer->End();
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

}; // namespace Pine
