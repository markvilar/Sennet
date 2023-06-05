#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "pine/core/event.hpp"

namespace pine
{

struct WindowSpecs
{
    std::string title{"Window"};
    uint32_t width{1280};
    uint32_t height{720};
    bool fullscreen{false};
    bool vsync{true};
};

class Window
{
    /*
    Interface class for Pines window implementations.
    */

public:
    using EventCallbackFn = std::function<void(const Event&)>;

    virtual ~Window() = default;

    virtual void init() = 0;
    virtual void poll_events() = 0;
    virtual void swap_buffers() = 0;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual std::pair<uint32_t, uint32_t> get_size() const = 0;
    virtual std::pair<float, float> get_position() const = 0;

    virtual void maximize() = 0;
    virtual void center_window() = 0;

    virtual void set_event_callback(const EventCallbackFn& callback) = 0;
    virtual void set_vsync(const bool enabled) = 0;
    virtual bool is_vsync() const = 0;
    virtual void set_resizable(const bool resizable) const = 0;

    virtual const std::string& get_title() const = 0;
    virtual void set_title(const std::string& title) = 0;

    virtual void* get_native_window() const = 0;
};

// Factory function, implement in platform
std::unique_ptr<Window> create_window(const WindowSpecs& specs);

} // namespace pine
