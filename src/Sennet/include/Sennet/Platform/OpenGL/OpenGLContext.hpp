#pragma once

#include "Sennet/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Sennet
{

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

} // namespace Sennet
