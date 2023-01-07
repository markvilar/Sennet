#pragma once

#include <memory>

#include "pine/core/window.hpp"

namespace pine::gui
{

// Forward declaration
class Context;
class Manager;

// Factory methods
std::unique_ptr<Context> create_context(Window* window);
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

class Manager
{
public:
    Manager(std::unique_ptr<Context>& context);
    ~Manager();

    void begin_frame() const;
    void end_frame() const;

    void on_event(Event& event) const;
    void block_events(const bool block) { handle_event = block; }

    std::string get_profile_name() const;
    void set_profile_name(const std::string& name);

private:
    std::unique_ptr<Context> context;
    std::string profile{"imgui.ini"};
    bool handle_event = true;
};

} // namespace pine::gui
