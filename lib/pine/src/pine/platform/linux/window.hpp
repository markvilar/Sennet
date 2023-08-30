#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "pine/core/window.hpp"
#include "pine/renderer/graphics_context.hpp"

namespace pine {

class LinuxWindow : public Window {
public:
    LinuxWindow(const WindowSpecs& specs);
    virtual ~LinuxWindow();

    virtual void init() override;
    virtual void poll_events() override;
    virtual void swap_buffers() override;

    inline uint32_t get_width() const override { return window_data.width; }
    inline uint32_t get_height() const override { return window_data.height; }
    virtual std::pair<uint32_t, uint32_t> get_size() const override;
    virtual std::pair<float, float> get_position() const override;

    virtual void maximize() override;
    virtual void center_window() override;

    virtual void set_event_callback(const EventCallbackFn& callback) override;
    virtual void set_resizable(const bool resizable) const override;

    virtual const std::string& get_title() const override;
    virtual void set_title(const std::string& title) override;

    virtual void* get_native_window() const override { return native_window; }

private:
    virtual void shutdown();
    void set_callbacks();

private:
    struct WindowData {
        std::string title;
        uint32_t width;
        uint32_t height;
        EventCallbackFn event_callback = [](const Event&) {};
    };

private:
    void* native_window;
    WindowSpecs specification;
    WindowData window_data;
};

} // namespace pine
