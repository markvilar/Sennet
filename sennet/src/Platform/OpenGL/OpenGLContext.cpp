#include "Sennet/Platform/OpenGL/OpenGLContext.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Core/Base.hpp"
#include "Sennet/Core/Log.hpp"

#include "glad/glad.h"

namespace Sennet
{

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle)
{
    SENNET_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    SENNET_CORE_ASSERT(status, "Failed to initialize Glad!")

    SENNET_CORE_INFO("OpenGL Info:");
    SENNET_CORE_INFO(" - Vendor:   {0}", glGetString(GL_VENDOR));
    SENNET_CORE_INFO(" - Renderer: {0}", glGetString(GL_RENDERER));
    SENNET_CORE_INFO(" - Version:  {0}", glGetString(GL_VERSION));

#ifdef SENNET_ENABLE_ASSERTS
    int version_major;
    int version_minor;
    glGetIntegerv(GL_MAJOR_VERSION, &version_major);
    glGetIntegerv(GL_MINOR_VERSION, &version_minor);

    SENNET_CORE_ASSERT(
        version_major > 4 || (version_major == 4 && version_minor >= 5),
        "Sennet requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace Sennet
