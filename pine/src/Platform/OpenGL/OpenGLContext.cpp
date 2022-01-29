#include "Pine/Platform/OpenGL/OpenGLContext.hpp"
#include "Pine/Pch.hpp"

#include "Pine/Core/Base.hpp"
#include "Pine/Core/Log.hpp"

#include "glad/glad.h"

namespace Pine
{

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle)
{
    PINE_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PINE_CORE_ASSERT(status, "Failed to initialize Glad!")

    PINE_CORE_INFO("OpenGL Info:");
    PINE_CORE_INFO(" - Vendor:   {0}", glGetString(GL_VENDOR));
    PINE_CORE_INFO(" - Renderer: {0}", glGetString(GL_RENDERER));
    PINE_CORE_INFO(" - Version:  {0}", glGetString(GL_VERSION));

#ifdef PINE_ENABLE_ASSERTS
    int version_major;
    int version_minor;
    glGetIntegerv(GL_MAJOR_VERSION, &version_major);
    glGetIntegerv(GL_MINOR_VERSION, &version_minor);

    PINE_CORE_ASSERT(
        version_major > 4 || (version_major == 4 && version_minor >= 5),
        "Pine requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace Pine
