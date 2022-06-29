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

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& e) override;

    void Begin();
    void End();

    void BlockEvents(const bool block) { m_BlockEvents = block; }

private:
    bool m_BlockEvents = true;
    float m_Time;
};

} // namespace pine
