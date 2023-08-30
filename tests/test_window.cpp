#include <iostream>
#include <memory>

#include <pine/pine.hpp>
#include <pine/graphics/api.hpp>
#include <pine/platform/glfw/window.hpp>

class TestInput : public pine::BasicInputHandle {
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

class TestWindow : public pine::BasicWindow {
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
    virtual std::unique_ptr<pine::BasicInputHandle> get_input () override {
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

    using Windows = std::vector<std::shared_ptr<pine::BasicWindow>>;

    // Inject GLFW components into a pine window context
    auto context = pine::WindowContext(
        pine::GlfwContext(),
        []() -> bool { return pine::glfw::load(); },
        []() -> bool { return pine::glfw::loaded(); },
        []() -> bool { return pine::glfw::unload(); }
    );

    context.load();

    Windows windows;
    windows.push_back(context.create_window());
    windows.push_back(context.create_window());
    windows.push_back(context.create_window());
    windows.push_back(context.create_window());
    windows.push_back(std::make_unique<TestWindow>());

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

    auto count = 0;
    while (context.is_loaded()) {
        PINE_CORE_INFO("Count: {0}", count++);
        for (auto window : windows) {
            window->swap_buffers();
        }
    }

    return 0;
};
