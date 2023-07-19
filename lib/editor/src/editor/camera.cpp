#include "editor/camera.hpp"

#include <imgui.h>

namespace pine {

void render_camera_controls(OrthographicCameraController& controller) {
    auto& parameters = controller.get_parameters();
    auto& pose = controller.get_pose();
    auto& aspect_ratio = controller.get_aspect_ratio();
    auto& zoom_level = controller.get_zoom_level();
    
    ImGui::Text("Parameters");

    gui::empty_space(0.0f, 2.0f);
    
    ImGui::Checkbox("Enable zoom", &parameters.zoom_enabled);
    ImGui::SameLine();
    ImGui::Checkbox("Enable rot.", &parameters.rotation_enabled);
    
    gui::empty_space(0.0f, 2.0f);

    ImGui::InputFloat("Zoom speed", &parameters.zoom_speed);
    ImGui::InputFloat("Min. zoom", &parameters.min_zoom);
    ImGui::InputFloat("Lin. speed", &parameters.linear_speed);
    ImGui::InputFloat("Rot. speed", &parameters.rotation_speed);

    gui::empty_space();

    ImGui::Text("Camera");
    ImGui::InputFloat3("Position", value_ptr(pose.position));
    ImGui::InputFloat("Rotation", &pose.rotation);
    ImGui::InputFloat("Aspect", &aspect_ratio);
    ImGui::InputFloat("Zoom", &zoom_level);
}

}; // namespace pine
