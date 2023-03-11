#include "pine/core/layer.hpp"

#include "pine/pch.hpp"

namespace pine
{

Layer::Layer(const std::string& name) : m_debug_name(name) {}

LayerStack::~LayerStack()
{
    for (Layer* layer : m_layers)
    {
        layer->on_detach();
        delete layer;
    }
}

void LayerStack::push_layer(Layer* layer)
{
    m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
    m_layer_insert_index++;
}

void LayerStack::push_overlay(Layer* overlay)
{
    m_layers.emplace_back(overlay);
}

void LayerStack::pop_layer(Layer* layer)
{
    auto it = std::find(m_layers.begin(),
        m_layers.begin() + m_layer_insert_index,
        layer);

    if (it != m_layers.begin() + m_layer_insert_index)
    {
        m_layers.erase(it);
        m_layer_insert_index--;
    }
}

void LayerStack::pop_overlay(Layer* overlay)
{
    auto it = std::find(m_layers.begin() + m_layer_insert_index,
        m_layers.end(),
        overlay);

    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
}

std::vector<Layer*>::iterator LayerStack::begin() { return m_layers.begin(); }

std::vector<Layer*>::iterator LayerStack::end() { return m_layers.end(); }

std::vector<Layer*>::reverse_iterator LayerStack::rbegin()
{
    return m_layers.rbegin();
}

std::vector<Layer*>::reverse_iterator LayerStack::rend()
{
    return m_layers.rend();
}

std::vector<Layer*>::const_iterator LayerStack::begin() const
{
    return m_layers.begin();
}

std::vector<Layer*>::const_iterator LayerStack::end() const
{
    return m_layers.end();
}

std::vector<Layer*>::const_reverse_iterator LayerStack::rbegin() const
{
    return m_layers.rbegin();
}

std::vector<Layer*>::const_reverse_iterator LayerStack::rend() const
{
    return m_layers.rend();
}

} // namespace pine
