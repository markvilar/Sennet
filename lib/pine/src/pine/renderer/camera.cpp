#include "pine/renderer/camera.hpp"

#include "pine/pch.hpp"

namespace pine
{

OrthographicCamera::OrthographicCamera(const float left,
    const float right,
    const float bottom,
    const float top)
    : projection_matrix(ortho(left, right, bottom, top, -1.0f, 1.0f)),
      view_matrix(1.0f)
{
}

void OrthographicCamera::set_projection(const float left,
    const float right,
    const float bottom,
    const float top)
{
    projection_matrix = ortho(left, right, bottom, top, -1.0f, 1.0f);
}

void OrthographicCamera::set_position(const Vec3& position_)
{
    position = position_;
    update_view_matrix();
}

void OrthographicCamera::set_rotation(const float rotation_)
{
    rotation = rotation_;
    update_view_matrix();
}

OrthographicCameraController::OrthographicCameraController(
    const float aspect_ratio_,
    const float zoom_level_,
    const Parameters& parameters_
)
    : aspect_ratio(aspect_ratio_),
    zoom_level(zoom_level_),
    parameters(parameters_),
    camera(-aspect_ratio_ * zoom_level_, 
        aspect_ratio_ * zoom_level_, 
        -zoom_level_, 
        zoom_level_
    )
{
}

void OrthographicCameraController::move_left(const Timestep& timestep)
{
    pose.position.x -= parameters.linear_speed * timestep;
}

void OrthographicCameraController::move_right(const Timestep& timestep)
{
    pose.position.x += parameters.linear_speed * timestep;
}

void OrthographicCameraController::move_up(const Timestep& timestep)
{
    pose.position.y += parameters.linear_speed * timestep;
}

void OrthographicCameraController::move_down(const Timestep& timestep)
{
    pose.position.y -= parameters.linear_speed * timestep;
}

void OrthographicCameraController::rotate_clockwise(const Timestep& timestep)
{
    if (parameters.rotation_enabled)
    {
        pose.rotation -= parameters.rotation_speed * timestep;
    }
}

void OrthographicCameraController::rotate_counter_clockwise(const Timestep& timestep)
{
    if (parameters.rotation_enabled)
    {
        pose.rotation += parameters.rotation_speed * timestep;
    }
}

void OrthographicCameraController::increment_zoom(const float increment)
{
    if (parameters.zoom_enabled)
    {
        // Update zoom
        zoom_level -= parameters.zoom_speed * increment;
        zoom_level = std::max(zoom_level, parameters.min_zoom);

        // Update linear speed based on zoom
        parameters.linear_speed = zoom_level;
    }
}

void OrthographicCameraController::set_aspect(const float width,
    const float height)
{
    aspect_ratio = width / height;
}

void OrthographicCameraController::update_camera()
{
    // TODO: Validate position and rotation
    camera.set_position(pose.position);
    camera.set_rotation(pose.rotation);
    camera.set_projection(-aspect_ratio * zoom_level,
        aspect_ratio * zoom_level,
        -zoom_level,
        zoom_level
    );
}

} // namespace pine
