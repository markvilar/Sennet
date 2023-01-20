#pragma once

#include "pine/core/common.hpp"
#include "pine/core/timestep.hpp"
#include "pine/core/window.hpp"
#include "pine/events/application_event.hpp"
#include "pine/events/mouse_event.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

class OrthographicCamera
{
public:
    OrthographicCamera(const float left,
        const float right,
        const float bottom,
        const float top);

    void set_projection(const float left,
        const float right,
        const float bottom,
        const float top);

    void set_position(const Vec3& position_)
    {
        position = position_;
        update_view_matrix();
    }

    void set_rotation(const float rotation_)
    {
        rotation = rotation_;
        update_view_matrix();
    }

    const Vec3& get_position() const { return position; }
    float get_rotation() const { return rotation; }

    const Mat4& get_projection_matrix() const { return projection_matrix; }
    const Mat4& get_view_matrix() const { return view_matrix; }

    Mat4 calculate_view_projection_matrix() const
    {
        return projection_matrix * view_matrix;
    }

private:
    void update_view_matrix()
    {
        Mat4 transform = translate(Mat4(1.0f), position)
            * rotate(Mat4(1.0f), rotation, Vec3(0, 0, 1));
        view_matrix = inverse(transform);
    }

private:
    Mat4 projection_matrix;
    Mat4 view_matrix;

    Vec3 position = {0.0f, 0.0f, 0.0f};
    float rotation = 0.0f; // Rotation in radians.
};

class OrthographicCameraController
{
public:
    OrthographicCameraController(const float aspectRatio,
        const bool rotation = false);

    void on_event(Event& event);

    void move_left(const Timestep& ts);
    void move_right(const Timestep& ts);
    void move_up(const Timestep& ts);
    void move_down(const Timestep& ts);

    void rotate_clockwise(const Timestep& ts);
    void rotate_counter_clockwise(const Timestep& ts);

    void on_resize(const float width, const float height);

    OrthographicCamera& get_camera() { return camera; }
    const OrthographicCamera& get_camera() const { return camera; }

    void set_zoom_level(const float level) { zoom_level = level; }
    float get_zoom_level() const { return zoom_level; }

private:
    bool on_mouse_scrolled(const MouseScrolledEvent& event);
    bool on_window_resized(const WindowResizeEvent& event);

private:
    float aspect_ratio;
    float zoom_level = 1.0f;
    bool rotation_enabled;

    OrthographicCamera camera;

    Vec3 camera_position = {0.0f, 0.0f, 0.0f};
    float camera_linear_speed = 1.5f;

    float camera_rotation = 0.0f; // Camera rotation in radians.
    float camera_rotation_speed = 1.0f;
};

} // namespace pine
