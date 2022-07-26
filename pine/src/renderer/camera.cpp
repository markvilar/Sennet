#include "pine/renderer/camera.hpp"

#include "pine/core/input.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/pch.hpp"

namespace pine
{

OrthographicCamera::OrthographicCamera(const float left, const float right,
    const float bottom, const float top)
    : m_projection_matrix(Ortho(left, right, bottom, top, -1.0f, 1.0f)),
      m_view_matrix(1.0f)
{
}

void OrthographicCamera::set_projection(const float left, const float right,
    const float bottom, const float top)
{
    m_projection_matrix = Ortho(left, right, bottom, top, -1.0f, 1.0f);
}

OrthographicCameraController::OrthographicCameraController(
    const float aspect_ratio, const bool rotation)
    : m_aspect_ratio(aspect_ratio), 
        m_rotation_enabled(rotation),
        m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level,
          -m_zoom_level, m_zoom_level)
{
}

void OrthographicCameraController::on_update(const Window& window,
    const Timestep& ts)
{
    if (input::is_key_pressed(window, KeyCode::A))
        m_camera_position.x -= m_camera_linear_speed * ts;
    else if (input::is_key_pressed(window, KeyCode::D))
        m_camera_position.x += m_camera_linear_speed * ts;

    if (input::is_key_pressed(window, KeyCode::W))
        m_camera_position.y += m_camera_linear_speed * ts;
    else if (input::is_key_pressed(window, KeyCode::S))
        m_camera_position.y -= m_camera_linear_speed * ts;

    if (m_rotation_enabled)
    {
        if (input::is_key_pressed(window, KeyCode::Q))
            m_camera_rotation += m_camera_rotation_speed * ts;
        if (input::is_key_pressed(window, KeyCode::E))
            m_camera_rotation -= m_camera_rotation_speed * ts;

        m_camera.set_rotation(m_camera_rotation);
    }

    m_camera.set_position(m_camera_position);

    m_camera_linear_speed = m_zoom_level;
}

void OrthographicCameraController::on_event(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::on_mouse_scrolled));
    dispatcher.dispatch<WindowResizeEvent>(
        PINE_BIND_EVENT_FN(OrthographicCameraController::on_window_resized));
}

void OrthographicCameraController::on_resize(const float width,
    const float height)
{
    m_aspect_ratio = width / height;
    m_camera.set_projection(-m_aspect_ratio * m_zoom_level,
        m_aspect_ratio * m_zoom_level,
        -m_zoom_level,
        m_zoom_level);
}

bool OrthographicCameraController::on_mouse_scrolled(
    const MouseScrolledEvent& event)
{
    m_zoom_level -= event.get_offset_y() * 0.25f;
    m_zoom_level = std::max(m_zoom_level, 0.25f);
    m_camera.set_projection(-m_aspect_ratio * m_zoom_level,
        m_aspect_ratio * m_zoom_level,
        -m_zoom_level,
        m_zoom_level);
    return false;
}

bool OrthographicCameraController::on_window_resized(
    const WindowResizeEvent& event)
{
    on_resize((float)event.get_width(), (float)event.get_height());
    return false;
}

} // namespace pine
