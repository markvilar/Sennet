#pragma once

#include "Sennet/Core/Layer.hpp"

#include "Sennet/Events/ApplicationEvent.hpp"
#include "Sennet/Events/KeyEvent.hpp"
#include "Sennet/Events/MouseEvent.hpp"

namespace Sennet
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

    void BlockEvents(bool block) { m_BlockEvents = block; }

private:
    bool m_BlockEvents = true;
    float m_Time;
};

} // namespace Sennet
