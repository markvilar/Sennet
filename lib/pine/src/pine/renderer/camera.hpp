#pragma once

#include "pine/core/timestep.hpp"
#include "pine/utils/math.hpp"

namespace pine {

class OrthographicCamera {
public:
    OrthographicCamera(const float left,
        const float right,
        const float bottom,
        const float top);

    void set_projection(const float left,
        const float right,
        const float bottom,
        const float top);

    void set_position(const Vec3& position);
    void set_rotation(const float rotation);

    Vec3 get_position() const { return position; }
    float get_rotation() const { return rotation; }

    Mat4 get_projection_matrix() const { return projection_matrix; }
    Mat4 get_view_matrix() const { return view_matrix; }

    Mat4 calculate_view_projection_matrix() const {
        return projection_matrix * view_matrix;
    }

private:
    void update_view_matrix() {
        Mat4 transform = translate(Mat4(1.0f), position) *
            rotate(Mat4(1.0f), rotation, Vec3(0, 0, 1));
        view_matrix = inverse(transform);
    }

private:
    Mat4 projection_matrix;
    Mat4 view_matrix;

    Vec3 position = {0.0f, 0.0f, 0.0f};
    float rotation = 0.0f; // Rotation in radians.
};

struct CameraControllerParameters {
    bool zoom_enabled = true;
    bool rotation_enabled = true;
    float zoom_speed = 0.25f;
    float min_zoom = 0.25f;
    float linear_speed = 1.5f;
    float rotation_speed = 1.0f;
};

struct CameraControllerPose {
    Vec3 position{0.0f, 0.0f, 0.0f};
    float rotation = 0.0f;
};

class OrthographicCameraController {
    using Camera = OrthographicCamera;
    using Pose = CameraControllerPose;
    using Parameters = CameraControllerParameters;

public:
    OrthographicCameraController(const float aspect_ratio,
        const float zoom_level,
        const Parameters& parameters = Parameters());

    Parameters get_parameters() const { return parameters; }
    Pose get_pose() const { return pose; }
    float get_aspect_ratio() const { return aspect_ratio; }
    float get_zoom_level() const { return zoom_level; }

    void set_parameters(const Parameters& parameters_) {
        parameters = parameters_;
    }
    void set_pose(const Pose& pose_) { pose = pose_; }
    void set_aspect_ratio(const float& aspect_ratio_) {
        aspect_ratio = aspect_ratio_;
    }
    void set_zoom_level(const float& level) { zoom_level = level; }

    void increment_zoom(const float increment);
    void set_aspect(const float width, const float height);

    Camera& get_camera() { return camera; }
    const Camera& get_camera() const { return camera; }

    // Controls
    void move_left(const Timestep& timestep);
    void move_right(const Timestep& timestep);
    void move_up(const Timestep& timestep);
    void move_down(const Timestep& timestep);

    void rotate_clockwise(const Timestep& timestep);
    void rotate_counter_clockwise(const Timestep& timestep);

    void update_camera();

private:
    Camera camera;
    Pose pose;
    Parameters parameters{};

    // Camera projection
    float aspect_ratio = 1.0f;
    float zoom_level = 1.0f;
};

} // namespace pine
