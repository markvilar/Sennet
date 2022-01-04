#include "Pine/Pch.hpp"
#include "Pine/Renderer/GraphicsContext.hpp"

#include "Pine/Platform/OpenGL/OpenGLContext.hpp"

namespace Pine
{

GraphicsContext* GraphicsContext::Create(void* window)
{
    // TODO: Add support for different renderer APIs later?
    return new OpenGLContext(static_cast<GLFWwindow*>(window));
}

} // namespace Pine
