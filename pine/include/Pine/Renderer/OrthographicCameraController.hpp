#pragma once

#include "Pine/Core/Base.hpp"
#include "Pine/Core/Timestep.hpp"
#include "Pine/Events/ApplicationEvent.hpp"
#include "Pine/Events/MouseEvent.hpp"
#include "Pine/Renderer/OrthographicCamera.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

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

} // namespace Pine
