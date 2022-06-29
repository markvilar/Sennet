#pragma once

#include "pine/core/common.hpp"
#include "pine/core/timestep.hpp"
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

    void SetProjection(const float left, const float right, const float bottom,
        const float top);

    const Vec3& GetPosition() const { return m_Position; }
    void SetPosition(const Vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(const float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

    const Mat4& GetViewMatrix() const { return m_ViewMatrix; }

    const Mat4& GetViewProjectionMatrix() const
    {
        return m_ViewProjectionMatrix;
    }

private:
    void RecalculateViewMatrix();

private:
    Mat4 m_ProjectionMatrix;
    Mat4 m_ViewMatrix;
    Mat4 m_ViewProjectionMatrix;

    Vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f; // Rotation in radians.
};


class OrthographicCameraController
{
public:
    OrthographicCameraController(const float aspectRatio,
        const bool rotation = false);

    void OnUpdate(const Timestep& ts);
    void OnEvent(Event& e);

    void OnResize(const float width, const float height);

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    void SetZoomLevel(const float level) { m_ZoomLevel = level; }
    float GetZoomLevel() const { return m_ZoomLevel; }

private:
    bool OnMouseScrolled(const MouseScrolledEvent& e);
    bool OnWindowResized(const WindowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    bool m_Rotation;

    OrthographicCamera m_Camera;

    Vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraTranslationSpeed = 1.5f;

    float m_CameraRotation = 0.0f; // Camera rotation in radians.
    float m_CameraRotationSpeed = 1.0f;
};

} // namespace pine
