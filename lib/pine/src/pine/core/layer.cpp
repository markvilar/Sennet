#include "pine/core/layer.hpp"

#include "pine/pch.hpp"

namespace pine
{

Layer::Layer(const std::string& name_) : name(name_) {}

LayerStack::~LayerStack()
{
    for (Layer* layer : layers)
    {
        layer->on_detach();
        delete layer;
    }
}

void LayerStack::push_layer(Layer* layer)
{
    layers.emplace(layers.begin() + layer_insert_index, layer);
    layer_insert_index++;
}

void LayerStack::push_overlay(Layer* overlay)
{
    layers.emplace_back(overlay);
}

void LayerStack::pop_layer(Layer* layer)
{
    auto it = std::find(layers.begin(),
        layers.begin() + layer_insert_index,
        layer);

    if (it != layers.begin() + layer_insert_index)
    {
        layers.erase(it);
        layer_insert_index--;
    }
}

void LayerStack::pop_overlay(Layer* overlay)
{
    auto it = std::find(layers.begin() + layer_insert_index,
        layers.end(),
        overlay);

    if (it != layers.end())
    {
        layers.erase(it);
    }
}

std::vector<Layer*>::iterator LayerStack::begin() { return layers.begin(); }

std::vector<Layer*>::iterator LayerStack::end() { return layers.end(); }

std::vector<Layer*>::reverse_iterator LayerStack::rbegin()
{
    return layers.rbegin();
}

std::vector<Layer*>::reverse_iterator LayerStack::rend()
{
    return layers.rend();
}

std::vector<Layer*>::const_iterator LayerStack::begin() const
{
    return layers.begin();
}

std::vector<Layer*>::const_iterator LayerStack::end() const
{
    return layers.end();
}

std::vector<Layer*>::const_reverse_iterator LayerStack::rbegin() const
{
    return layers.rbegin();
}

std::vector<Layer*>::const_reverse_iterator LayerStack::rend() const
{
    return layers.rend();
}

} // namespace pine
