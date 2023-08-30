#pragma once

#include <memory>

#include "pine/core/window.hpp"
#include "pine/renderer/graphics_context.hpp"

namespace pine {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(std::shared_ptr<Window>& window);

    virtual void init() override;
    virtual void set_vsync(const bool enabled) override;

private:
    std::weak_ptr<Window> window;
};

} // namespace pine
