#include "Pine/Renderer/GraphicsContext.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLContext.hpp"

namespace Pine
{

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
{
    return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
}

} // namespace Pine
