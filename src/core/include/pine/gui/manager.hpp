#pragma once

#include <memory>

#include "pine/core/window.hpp"
#include "pine/gui/common.hpp"
#include "pine/gui/style.hpp"
#include "pine/utils/filesystem.hpp"

namespace pine::gui
{

// Forward declaration
class Context;
class IO;
class Manager;

// Factory methods
std::unique_ptr<Context> create_context(Window* window);
std::unique_ptr<IO> create_io(Context* context);
std::unique_ptr<Manager> create_manager(Window* window);

class Context
{
public:
    Context(Window* window);
    ~Context();

    void init() const;
    void shutdown() const;

    void begin_frame() const;
    void end_frame() const;

private:
    Window* window{};
};

class IO
{
public:
    IO(Context* context);
    ~IO() = default;

    ConfigFlags get_config_flags() const;
    void set_config_flags(const ConfigFlags& config) const;

    bool load_settings(const std::filesystem::path& filepath) const;
    bool save_settings(const std::filesystem::path& filepath) const;

    bool
    load_font(const void* data, const int size, const float pixel_size) const;
    bool load_font(const std::filesystem::path& filepath,
        const float pixel_size) const;

    bool want_capture_mouse() const;
    bool want_capture_keyboard() const;

private:
    Context* context{};
};

class Manager
{
public:
    Manager(std::unique_ptr<Context>& context, std::unique_ptr<IO>& io);
    ~Manager();

    void begin_frame() const;
    void end_frame() const;

    bool load_settings(const std::filesystem::path& filepath) const;
    bool save_settings(const std::filesystem::path& filepath) const;

    bool
    load_font(const void* data, const int size, const float pixel_size) const;
    bool load_font(const std::filesystem::path& filepath,
        const float pixel_size) const;

    void on_event(Event& event) const;
    void block_events(const bool block) { handle_event = block; }

private:
    std::unique_ptr<Context> context;
    std::unique_ptr<IO> io;
    bool handle_event = true;
};

} // namespace pine::gui
