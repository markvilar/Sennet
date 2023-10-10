#include <iostream>
#include <memory>

#include <pine/pine.hpp>
#include <pine/graphics/api.hpp>

#include <pine/platform/glfw/api.hpp>
#include <pine/platform/glfw/window.hpp>

class TestInput : public pine::InputHandleBase {
public:
    virtual bool is_key_pressed(const pine::KeyCode key) override { 
        return false; 
    }
    virtual bool is_mouse_button_pressed(const pine::MouseCode button) override { 
        return false; 
    }
    virtual std::pair<float, float> get_mouse_position() override { 
        return { 0.0f, 0.0f }; 
    }
};

class TestWindow : public pine::WindowBase {
public:
    virtual ~TestWindow() = default;
    virtual std::string_view get_title() override {
        return "Test Window";
    }
    virtual std::pair<uint32_t, uint32_t> get_size() override {
        return { 0, 0 };
    }
    virtual std::pair<uint32_t, uint32_t> get_position() override {
        return { 0, 0 };
    }
    virtual std::unique_ptr<pine::InputHandleBase> get_input () override {
        return std::make_unique<TestInput>();
    }
    virtual void swap_buffers() override {}
    virtual void maximize() override {}
    virtual void center() override {}
    virtual void set_event_callback(const std::function<void(const pine::Event&)>&) override  {}
};

int main(int argc, char** argv)
{
    pine::Log::init();

    using Windows = std::vector<std::shared_ptr<pine::WindowBase>>;

    // Inject GLFW components into a pine window system
    auto system = pine::WindowSystem(
        pine::GlfwFactory(),
        []() -> bool { return pine::glfw::load(); },
        []() -> bool { return pine::glfw::is_loaded(); },
        []() -> bool { return pine::glfw::unload(); }
    );

    // Call the injected load function
    system.load();

    // Create and push windows into a vector
    Windows windows;
    windows.push_back(system.create_window());
    windows.push_back(system.create_window());
    windows.push_back(system.create_window());
    windows.push_back(system.create_window());
    windows.push_back(std::make_unique<TestWindow>());

    // Get window names and mouse positions
    auto window_count = 0;
    for (auto window : windows) {
        if (window) {
            window_count++;
            auto input = window->get_input();
            const auto position = input->get_mouse_position();

            PINE_CORE_INFO("Window {0}: {1}, {2}, {3}", 
                window_count, 
                window->get_title(),
                position.first,
                position.second
            );

        }
    }

    // Count frames while the window system is loaded
    auto count = 0;
    while (system.is_loaded()) {
        PINE_CORE_INFO("Count: {0}", count++);
        for (auto window : windows) {
            window->swap_buffers();
        }
    }

    return 0;
};
