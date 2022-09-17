#include "pine/renderer/camera.hpp"

#include "pine/core/input.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/pch.hpp"

namespace pine
{

OrthographicCamera::OrthographicCamera(const float left, const float right,
    const float bottom, const float top)
    : projection_matrix(ortho(left, right, bottom, top, -1.0f, 1.0f)),
      view_matrix(1.0f)
{
}

void OrthographicCamera::set_projection(const float left, const float right,
    const float bottom, const float top)
{
    projection_matrix = ortho(left, right, bottom, top, -1.0f, 1.0f);
}

OrthographicCameraController::OrthographicCameraController(
    const float aspect_ratio, const bool rotation)
    : aspect_ratio(aspect_ratio), rotation_enabled(rotation),
      camera(-aspect_ratio * zoom_level, aspect_ratio * zoom_level,
          -zoom_level, zoom_level)
{
}

void OrthographicCameraController::move_left(const Timestep& ts)
{
    camera_position.x -= camera_linear_speed * ts;
    camera.set_position(camera_position);
}

void OrthographicCameraController::move_right(const Timestep& ts)
{
    camera_position.x += camera_linear_speed * ts;
    camera.set_position(camera_position);
}

void OrthographicCameraController::move_up(const Timestep& ts)
{
    camera_position.y += camera_linear_speed * ts;
    camera.set_position(camera_position);
}

void OrthographicCameraController::move_down(const Timestep& ts)
{
    camera_position.y -= camera_linear_speed * ts;
    camera.set_position(camera_position);
}

void OrthographicCameraController::rotate_clockwise(const Timestep& ts)
{
    if (rotation_enabled)
    {
        camera_rotation -= camera_rotation_speed * ts;
        camera.set_rotation(camera_rotation);
    }
}

void OrthographicCameraController::rotate_counter_clockwise(const Timestep& ts)
{
    if (rotation_enabled)
    {
        camera_rotation += camera_rotation_speed * ts;
        camera.set_rotation(camera_rotation);
    }
}

void OrthographicCameraController::on_event(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(
        [this](const MouseScrolledEvent& event) -> bool
        {
            return on_mouse_scrolled(event);
        });
        
    // PINE_BIND_EVENT_FN(OrthographicCameraController::on_mouse_scrolled));
    dispatcher.dispatch<WindowResizeEvent>(
        [this](const WindowResizeEvent& event) -> bool
        {
            return on_window_resized(event);
        });
        
    //PINE_BIND_EVENT_FN(OrthographicCameraController::on_window_resized));
}

void OrthographicCameraController::on_resize(const float width,
    const float height)
{
    aspect_ratio = width / height;
    camera.set_projection(-aspect_ratio * zoom_level,
        aspect_ratio * zoom_level,
        -zoom_level,
        zoom_level);
}

bool OrthographicCameraController::on_mouse_scrolled(
    const MouseScrolledEvent& event)
{
    zoom_level -= event.get_offset_y() * 0.25f;
    zoom_level = std::max(zoom_level, 0.25f);
    camera_linear_speed = zoom_level;
    camera.set_projection(-aspect_ratio * zoom_level,
        aspect_ratio * zoom_level,
        -zoom_level,
        zoom_level);
    return false;
}

bool OrthographicCameraController::on_window_resized(
    const WindowResizeEvent& event)
{
    on_resize(static_cast<float>(event.get_width()),
        static_cast<float>(event.get_height()));
    return false;
}

} // namespace pine
