#pragma once

#include <memory>

#include "pine/core/window.hpp"

namespace pine
{

class GraphicalInterface
{
public:
    GraphicalInterface(Window* window);
    ~GraphicalInterface();

    void begin_frame();
    void end_frame();

    void on_event(Event& event);

    void block_events(const bool block) { m_block_events = block; }

    // Factory method
    static std::unique_ptr<GraphicalInterface> create(Window* window);

private:
    Window* m_window;
    bool m_block_events = true;
};

} // namespace pine::ui
