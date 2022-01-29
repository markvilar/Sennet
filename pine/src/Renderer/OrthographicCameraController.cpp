#include "Pine/Renderer/OrthographicCameraController.hpp"

#include "Pine/Core/Input.hpp"
#include "Pine/Core/KeyCodes.hpp"
#include "Pine/Pch.hpp"

namespace Pine
{

OrthographicCameraController::OrthographicCameraController(
    const float aspectRatio, const bool rotation)
    : m_AspectRatio(aspectRatio), m_Rotation(rotation),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
          -m_ZoomLevel, m_ZoomLevel)
{
}

void OrthographicCameraController::OnUpdate(const Timestep& ts)
{
    if (Input::IsKeyPressed(KeyCode::A))
        m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
    else if (Input::IsKeyPressed(KeyCode::D))
        m_CameraPosition.x += m_CameraTranslationSpeed * ts;

    if (Input::IsKeyPressed(KeyCode::W))
        m_CameraPosition.y += m_CameraTranslationSpeed * ts;
    else if (Input::IsKeyPressed(KeyCode::S))
        m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

    if (m_Rotation)
    {
        if (Input::IsKeyPressed(KeyCode::Q))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Input::IsKeyPressed(KeyCode::E))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(
    const float width, const float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
        m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
        m_ZoomLevel);
}

bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& e)
{
    m_ZoomLevel -= e.GetOffsetY() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
        m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
        m_ZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& e)
{
    OnResize((float)e.GetWidth(), (float)e.GetHeight());
    return false;
}

} // namespace Pine
