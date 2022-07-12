#pragma once

#include "pine/core/common.hpp"
#include "pine/core/timestep.hpp"
#include "pine/events/event.hpp"

namespace pine
{

class Layer
{
public:
    Layer(const std::string& name = "layer");
    virtual ~Layer() = default;

    virtual void on_attach() {}
    virtual void on_detach() {}
    virtual void on_update(Timestep ts) {}
    virtual void on_imgui_render() {}
    virtual void on_event(Event& event) {}

    inline const std::string& get_name() const { return m_debug_name; }

protected:
    std::string m_debug_name;
};

} // namespace pine
