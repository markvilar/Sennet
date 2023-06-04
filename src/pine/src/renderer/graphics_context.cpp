#include "graphics_context.hpp"

#include "pine/pch.hpp"

#include "../platform/opengl/context.hpp"

namespace pine
{

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
{
    return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
}

} // namespace pine
