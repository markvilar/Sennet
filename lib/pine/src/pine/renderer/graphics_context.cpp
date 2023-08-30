#include "pine/renderer/graphics_context.hpp"

#include "pine/pch.hpp"

#include "pine/platform/opengl/context.hpp"

namespace pine {

std::unique_ptr<GraphicsContext> GraphicsContext::create(std::shared_ptr<Window>& window) {
    if (window)
    {
        return std::make_unique<OpenGLContext>(window);
    }
    return nullptr;
}

} // namespace pine
