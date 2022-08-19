#include "pine/platform/opengl/context.hpp"

#include <glad/glad.h>

#include "pine/core/common.hpp"
#include "pine/core/log.hpp"
#include "pine/pch.hpp"

namespace pine
{

OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    : m_window_handle(window_handle)
{
    PINE_CORE_ASSERT(window_handle, "Window handle is null!")
}

void OpenGLContext::init()
{
    glfwMakeContextCurrent(m_window_handle);
    const auto status =
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    PINE_CORE_ASSERT(status, "Failed to initialize Glad!")

    PINE_CORE_INFO("OpenGL Info:");
    PINE_CORE_INFO(" - Vendor:   {0}", glGetString(GL_VENDOR));
    PINE_CORE_INFO(" - Renderer: {0}", glGetString(GL_RENDERER));
    PINE_CORE_INFO(" - Version:  {0}", glGetString(GL_VERSION));

#ifdef PINE_ENABLE_ASSERTS
    int version_major, version_minor;
    glGetIntegerv(GL_MAJOR_VERSION, &version_major);
    glGetIntegerv(GL_MINOR_VERSION, &version_minor);

    PINE_CORE_ASSERT(version_major > 4
            || (version_major == 4 && version_minor >= 5),
        "pine requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::swap_buffers() { glfwSwapBuffers(m_window_handle); }

} // namespace pine
