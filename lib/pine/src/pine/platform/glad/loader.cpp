#include "pine/platform/glad/loader.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/core/log.hpp"
#include "pine/defines/assert.hpp"
#include "pine/pch.hpp"

namespace pine {

bool load_opengl(const GlfwContext& context) {
    const auto status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    PINE_CORE_VERIFY(status, "Failed to initialize Glad!");

    if (status) {
        PINE_CORE_INFO("Glad loader:");
        PINE_CORE_INFO(" - Vendor:   {0}", get_opengl_vendor());
        PINE_CORE_INFO(" - Renderer: {0}", get_opengl_renderer());
        PINE_CORE_INFO(" - Version:  {0}", get_opengl_version());
    }

    return status;
}

bool opengl_loaded() {
    return gladLoadGL();
}

std::string get_opengl_vendor() {
    return std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
}

std::string get_opengl_renderer() {
    return std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

std::string get_opengl_version() {
    return std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

} // namespace pine
