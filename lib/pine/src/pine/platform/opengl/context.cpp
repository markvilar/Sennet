#include "pine/platform/opengl/context.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/core/log.hpp"
#include "pine/defines/assert.hpp"
#include "pine/pch.hpp"

namespace pine {

OpenGLContext::OpenGLContext(std::shared_ptr<Window>& window_)
    : window(window_) {
    PINE_CORE_VERIFY(window_, "Window handle is null!");
}

void OpenGLContext::init() {
    auto instance = window.lock();
    auto native_window = static_cast<GLFWwindow*>(instance->get_native_window());
    glfwMakeContextCurrent(native_window);
    [[maybe_unused]] const auto status =
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    PINE_CORE_ASSERT(status, "Failed to initialize Glad!");

    PINE_CORE_INFO("OpenGL Context:");
    PINE_CORE_INFO(" - Vendor:   {0}", glGetString(GL_VENDOR));
    PINE_CORE_INFO(" - Renderer: {0}", glGetString(GL_RENDERER));
    PINE_CORE_INFO(" - Version:  {0}", glGetString(GL_VERSION));

// #ifdef PINE_ENABLE_ASSERTS
    int version_major, version_minor;
    glGetIntegerv(GL_MAJOR_VERSION, &version_major);
    glGetIntegerv(GL_MINOR_VERSION, &version_minor);

    PINE_CORE_ASSERT(
        version_major > 4 || (version_major == 4 && version_minor >= 5),
        "pine requires at least OpenGL version 4.5!");
// #endif
}
    
void OpenGLContext::set_vsync(const bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}


} // namespace pine
