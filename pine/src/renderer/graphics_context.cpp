#include "pine/renderer/graphics_context.hpp"

#include "pine/platform/opengl/context.hpp"
#include "pine/pch.hpp"

namespace pine
{

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
{
    return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
}

} // namespace pine
