#pragma once

#include <GLFW/glfw3.h>

#include "pine/renderer/graphics_context.hpp"

namespace pine
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

} // namespace pine
