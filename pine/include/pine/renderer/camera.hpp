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
    OrthographicCamera(const float left, const float right, const float bottom,
        const float top);

    void set_projection(const float left, const float right, const float bottom,
        const float top);
    
    void set_position(const Vec3& position) 
    { 
        m_position = position;
        update_view_matrix();
    }
    
    void set_rotation(const float rotation) 
    { 
        m_rotation = rotation; 
        update_view_matrix();
    }

    const Vec3& get_position() const { return m_position; }
    float get_rotation() const { return m_rotation; }

    const Mat4& get_projection_matrix() const { return m_projection_matrix; }
    const Mat4& get_view_matrix() const { return m_view_matrix; }

    Mat4 calculate_view_projection_matrix() const
    {
        return m_projection_matrix * m_view_matrix;
    }

private:
    void update_view_matrix()
    {
        Mat4 transform = Translate(Mat4(1.0f), m_position)
            * Rotate(Mat4(1.0f), m_rotation, Vec3(0, 0, 1));
        m_view_matrix = Inverse(transform);
    }

private:
    Mat4 m_projection_matrix;
    Mat4 m_view_matrix;

    Vec3 m_position = {0.0f, 0.0f, 0.0f};
    float m_rotation = 0.0f; // Rotation in radians.
};


class OrthographicCameraController
{
public:
    OrthographicCameraController(const float aspectRatio,
        const bool rotation = false);

    void on_update(const Window& window, const Timestep& ts);
    void on_event(Event& event);

    void on_resize(const float width, const float height);

    OrthographicCamera& get_camera() { return m_camera; }
    const OrthographicCamera& get_camera() const { return m_camera; }

    void set_zoom_level(const float level) { m_zoom_level = level; }
    float get_zoom_level() const { return m_zoom_level; }

private:
    bool on_mouse_scrolled(const MouseScrolledEvent& event);
    bool on_window_resized(const WindowResizeEvent& event);

private:
    float m_aspect_ratio;
    float m_zoom_level = 1.0f;
    bool m_rotation_enabled;

    OrthographicCamera m_camera;

    Vec3 m_camera_position = {0.0f, 0.0f, 0.0f};
    float m_camera_linear_speed = 1.5f;

    float m_camera_rotation = 0.0f; // Camera rotation in radians.
    float m_camera_rotation_speed = 1.0f;
};

} // namespace pine
