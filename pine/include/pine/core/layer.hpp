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

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& e) {}

    inline const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

} // namespace pine
