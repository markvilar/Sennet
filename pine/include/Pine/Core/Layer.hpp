#pragma once

#include "Pine/Core/Base.hpp"
#include "Pine/Core/Timestep.hpp"
#include "Pine/Events/Event.hpp"

namespace Pine
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

} // namespace Pine
