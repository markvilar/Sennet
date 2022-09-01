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
    virtual void on_update([[maybe_unused]] Timestep ts) {}
    virtual void on_gui_render() {}
    virtual void on_event([[maybe_unused]] Event& event) {}

    inline const std::string& get_name() const { return m_debug_name; }

protected:
    std::string m_debug_name;
};

class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    void push_layer(Layer* layer);
    void push_overlay(Layer* overlay);
    void pop_layer(Layer* layer);
    void pop_overlay(Layer* overlay);

    std::vector<Layer*>::iterator begin();
    std::vector<Layer*>::iterator end();
    std::vector<Layer*>::reverse_iterator rbegin();
    std::vector<Layer*>::reverse_iterator rend();

    std::vector<Layer*>::const_iterator begin() const;
    std::vector<Layer*>::const_iterator end() const;
    std::vector<Layer*>::const_reverse_iterator rbegin() const;
    std::vector<Layer*>::const_reverse_iterator rend() const;

private:
    std::vector<Layer*> m_layers;
    unsigned int m_layer_insert_index = 0;
};

} // namespace pine
