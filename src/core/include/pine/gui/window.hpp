#pragma once

#include <string>

#include "pine/utils/math.hpp"

namespace pine::gui
{
using WindowFlags = int;

class Window
{
public:
    Window(const std::string& name);
    ~Window() = default;

    template <typename Callback>
    void on_render(const Callback& callback)
    {
        begin();
        callback();
        update_state();
        end();
    }

    void on_render()
    {
        begin();
        update_state();
        end();
    }

    Vec2 get_position() const { return position; }
    Vec2 get_size() const { return size; }

    bool is_focused() const { return focused; }
    bool is_hovered() const { return hovered; }

private:
    void begin() const;
    void end() const;

    void update_state();

private:
    std::string name{};
    Vec2 position{};
    Vec2 size{};
    bool focused{};
    bool hovered{};
};

} // namespace pine::gui
