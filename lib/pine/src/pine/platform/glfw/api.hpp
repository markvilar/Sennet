#pragma once

#include <cstdint>
#include <string>

#include "pine/core/codes.hpp"

// Forward declaration
struct GLFWwindow;

namespace pine::glfw {

/*
Backend loading functions:
- load
- loaded
- unload
*/
bool load() noexcept;
bool is_loaded() noexcept;
bool unload() noexcept;

/*
Input functions:
- is_key_pressed
- is_mouse_button_pressed
- get_mouse_position
- poll_events
*/
bool is_key_pressed(GLFWwindow* window, const KeyCode key) noexcept;
bool is_mouse_button_pressed(GLFWwindow* window, const MouseCode button) noexcept;
std::pair<float, float> get_mouse_position(GLFWwindow* window) noexcept;
void poll_events() noexcept;

/*
Window functions:
- create_window
- set_context
- has_context
- set_vsync
*/
GLFWwindow* create_window(
    const uint32_t width, 
    const uint32_t height,
    const std::string& title
) noexcept;
uint8_t get_window_count() noexcept;
void set_context(GLFWwindow* window) noexcept;
bool has_context() noexcept;
void set_vsync(const bool enabled) noexcept;

/*
Graphics loading functions:
- load_opengl
- opengl_loaded
*/
bool load_opengl() noexcept;
bool opengl_loaded() noexcept;

}
