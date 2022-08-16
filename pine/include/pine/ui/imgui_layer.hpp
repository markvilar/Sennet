#pragma once

#include "pine/core/layer.hpp"
#include "pine/events/application_event.hpp"
#include "pine/events/key_event.hpp"
#include "pine/events/mouse_event.hpp"

namespace pine
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_event(Event& event) override;

    void begin();
    void end();

    void block_events(const bool block) { m_block_events = block; }

private:
    bool m_block_events = true;
    float m_time;
};

} // namespace pine
