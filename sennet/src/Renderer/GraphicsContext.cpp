#include "Sennet/Renderer/GraphicsContext.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Platform/OpenGL/OpenGLContext.hpp"

namespace Sennet
{

GraphicsContext* GraphicsContext::Create(void* window)
{
    // TODO: Add support for different renderer APIs later?
    return new OpenGLContext(static_cast<GLFWwindow*>(window));
}

} // namespace Sennet
