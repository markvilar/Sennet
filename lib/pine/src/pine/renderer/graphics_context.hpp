#pragma once

#include <memory>

#include <pine/core/window.hpp>

namespace pine {

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    virtual void init() = 0;
    virtual void set_vsync(const bool enabled) = 0;

    static std::unique_ptr<GraphicsContext> create(std::shared_ptr<Window>& window);
};

} // namespace pine
