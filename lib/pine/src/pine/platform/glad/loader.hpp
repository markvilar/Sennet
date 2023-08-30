#pragma once

#include "pine/platform/glfw/window.hpp"

namespace pine {

/*
Opengl loading API:
 - load_opengl
 - opengl_loaded
*/

bool load_opengl(const GlfwContext& context);
bool opengl_loaded();

/*
Opengl device info:
 - get_opengl_vendor
 - get_opengl_renderer
 - get_opengl_version
*/

std::string get_opengl_vendor();
std::string get_opengl_renderer();
std::string get_opengl_version();

} // namespace pine
