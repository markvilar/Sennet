#pragma once

#include "Pine/Core/Layer.hpp"
#include "Pine/Events/ApplicationEvent.hpp"
#include "Pine/Events/KeyEvent.hpp"
#include "Pine/Events/MouseEvent.hpp"

namespace Pine
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

} // namespace Pine
