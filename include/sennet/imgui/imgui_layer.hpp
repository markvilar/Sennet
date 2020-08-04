#pragma once

#include <sennet/core/layer.hpp>

#include <sennet/events/application_event.hpp>
#include <sennet/events/key_event.hpp>
#include <sennet/events/mouse_event.hpp>

namespace sennet
{

class imgui_layer : public layer
{
public:
	imgui_layer();
	~imgui_layer() = default;

	void on_attach();
	void on_detach();
	void on_update();
	void on_event(event& e);

private:
	bool on_mouse_button_pressed_event(mouse_button_pressed_event& e);
	bool on_mouse_button_released_event(mouse_button_released_event& e);
	bool on_mouse_moved_event(mouse_moved_event& e);
	bool on_mouse_scrolled_event(mouse_scrolled_event& e);

	bool on_key_pressed_event(key_pressed_event& e);
	bool on_key_released_event(key_released_event& e);
	bool on_key_typed_event(key_typed_event& e);

	bool on_window_resized_event(window_resize_event& e);

private:
	float m_time;

};

}
