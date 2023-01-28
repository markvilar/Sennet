#pragma once

#include <GLFW/glfw3.h>

#include "pine/renderer/graphics_context.hpp"

namespace pine
{
class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void init() override;
    virtual void swap_buffers() override;

private:
    GLFWwindow* window_handle;
};

} // namespace pine
