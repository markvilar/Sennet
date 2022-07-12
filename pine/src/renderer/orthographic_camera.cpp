#include "pine/renderer/orthographic_camera.hpp"

#include "pine/core/input.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/pch.hpp"

namespace pine
{

OrthographicCamera::OrthographicCamera(const float left, const float right,
    const float bottom, const float top)
    : m_ProjectionMatrix(Ortho(left, right, bottom, top, -1.0f, 1.0f)),
      m_ViewMatrix(1.0f)
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetProjection(const float left, const float right,
    const float bottom, const float top)
{
    m_ProjectionMatrix = Ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateViewMatrix()
{
    Mat4 transform = Translate(Mat4(1.0f), m_Position)
        * Rotate(Mat4(1.0f), m_Rotation, Vec3(0, 0, 1));

    m_ViewMatrix = Inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

OrthographicCameraController::OrthographicCameraController(
    const float aspectRatio, const bool rotation)
    : m_AspectRatio(aspectRatio), m_Rotation(rotation),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
          -m_ZoomLevel, m_ZoomLevel)
{
}

void OrthographicCameraController::OnUpdate(const Window& window,
    const Timestep& ts)
{
    if (input::is_key_pressed(window, KeyCode::A))
        m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
    else if (input::is_key_pressed(window, KeyCode::D))
        m_CameraPosition.x += m_CameraTranslationSpeed * ts;

    if (input::is_key_pressed(window, KeyCode::W))
        m_CameraPosition.y += m_CameraTranslationSpeed * ts;
    else if (input::is_key_pressed(window, KeyCode::S))
        m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

    if (m_Rotation)
    {
        if (input::is_key_pressed(window, KeyCode::Q))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (input::is_key_pressed(window, KeyCode::E))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.dispatch<WindowResizeEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(const float width,
    const float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
        m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
        m_ZoomLevel);
}

bool OrthographicCameraController::OnMouseScrolled(
    const MouseScrolledEvent& event)
{
    m_ZoomLevel -= event.GetOffsetY() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
        m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
        m_ZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(
    const WindowResizeEvent& event)
{
    OnResize((float)event.GetWidth(), (float)event.GetHeight());
    return false;
}

} // namespace pine
