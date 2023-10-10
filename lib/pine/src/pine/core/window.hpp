#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "pine/core/codes.hpp"
#include "pine/core/event.hpp"

namespace pine {

struct WindowSpecs {
    std::string title{"Window"};
    uint32_t width{1280};
    uint32_t height{720};
    bool fullscreen{false};
    bool vsync{true};
};

template <typename Callback>
concept EventCallback = requires(Callback callback, Event& event) {
    std::is_invocable_v<Callback, Event&>;
};

class Window {
    /*
    TODO: Deprecated class. Replace with new WindowBase.
    Interface class for Pines window implementations.
    */

public:
    using EventCallbackFn = std::function<void(const Event&)>;

    virtual ~Window() = default;

    virtual void init() = 0;
    virtual void poll_events() = 0;
    virtual void swap_buffers() = 0;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual std::pair<uint32_t, uint32_t> get_size() const = 0;
    virtual std::pair<float, float> get_position() const = 0;

    virtual void maximize() = 0;
    virtual void center_window() = 0;

    virtual void set_event_callback(const EventCallbackFn& callback) = 0;
    virtual void set_resizable(const bool resizable) const = 0;

    virtual const std::string& get_title() const = 0;
    virtual void set_title(const std::string& title) = 0;

    virtual void* get_native_window() const = 0;
};

// Factory function, implement in platform
std::unique_ptr<Window> create_window(const WindowSpecs& specs);

// NOTE: New design idea

template <typename T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

template <typename T>
concept InputConcept = requires(T t, const KeyCode key, const MouseCode button) {
    t.is_key_pressed(key);
    t.is_mouse_button_pressed(button);
    t.get_mouse_position();
};

template <typename T>
concept WindowConcept = requires(T t, const std::function<void(const Event&)>& callback) {
    typename T::InputType;
    { t.get_title() } -> StringLike;
    t.get_size();
    t.get_position();
    { t.get_input() } -> InputConcept;
    t.swap_buffers();
    t.maximize();
    t.center();
    t.set_event_callback(callback);
};

template <typename T>
concept WindowFactoryConcept = requires(T t) {
    typename T::WindowType;
    typename T::InputType;
    t.create_window();
};

class InputHandleBase {
public:
    virtual ~InputHandleBase() = default;
    virtual bool is_key_pressed(const KeyCode key) = 0;
    virtual bool is_mouse_button_pressed(const MouseCode button) = 0;
    virtual std::pair<float, float> get_mouse_position() = 0;
};

/*
InputHandleWrapper:
Wrapper for an input handle based on external polymorphism.
*/
template <InputConcept InputImpl>
class InputHandleWrapper final : public InputHandleBase {
public:
    InputHandleWrapper(InputImpl impl_) : impl(std::move(impl_)) {}
    virtual ~InputHandleWrapper() = default;
    
    virtual bool is_key_pressed(const KeyCode key) override {
        return impl.is_key_pressed(key);
    }

    virtual bool is_mouse_button_pressed(const MouseCode button) override {
        return impl.is_mouse_button_pressed(button);
    }

    virtual std::pair<float, float> get_mouse_position() override {
        return impl.get_mouse_position();
    }

private:
    InputImpl impl;
};

/*
WindowBase:
Base class for a pine window.
*/
class WindowBase {
public:
    using size_t = std::uint32_t;

    virtual ~WindowBase() = default;
    virtual std::string_view get_title() = 0;
    virtual std::pair<size_t, size_t> get_size() = 0;
    virtual std::pair<size_t, size_t> get_position() = 0;
    virtual std::unique_ptr<InputHandleBase> get_input() = 0;
    virtual void swap_buffers() = 0;
    virtual void maximize() = 0;
    virtual void center() = 0;
    virtual void set_event_callback(const std::function<void(const Event&)>&) = 0;
};

/*
WindowFactoryBase:
Base class for a window factory. 
*/
class WindowFactoryBase {
public:
    virtual ~WindowFactoryBase() = default;
    virtual std::shared_ptr<WindowBase> create_window() = 0;
};

/*
WindowSystem:
Class for a window system consisting of window factory and a backend loader. 
- The backend loader is responsible for loading and unloading the window backend
- The factory is reponsible for window creation
- The client is responsible for maintaining window lifetime
*/

class WindowSystem {

using BackendLoader = std::function<bool(void)>;
using BackendObserver = std::function<bool(void)>;
using BackendUnloader = std::function<bool(void)>;

public:
    template <WindowFactoryConcept Factory>
    WindowSystem(
        Factory factory_, 
        const BackendLoader& loader_,
        const BackendObserver& observer_,
        const BackendUnloader& unloader_
    ) 
    : factory(std::make_unique<WindowFactoryWrapper<Factory>>(std::move(factory_))),
        loader(loader_), observer(observer_), unloader(unloader_) {}

    bool load() const noexcept { return loader(); }
    bool is_loaded() const noexcept { return observer(); }
    bool unload() const noexcept { return unloader(); }

    /* Factory */
    std::shared_ptr<WindowBase> create_window() {
        return factory->create_window();
    }

private:

/*
WindowWrapper:
Wrapper for a window based on external polymorphism. The wrapper is responsible
for dispatching function calls to the window implementation.
*/
template <WindowConcept WindowImpl>
class WindowWrapper final : public WindowBase {
public:
    WindowWrapper(std::unique_ptr<WindowImpl> impl_) : impl(std::move(impl_)) {}

    virtual std::string_view get_title() override {
        return impl->get_title();
    }
    
    virtual std::pair<uint32_t, uint32_t> get_size() override {
        return impl->get_size();
    }
    
    virtual std::pair<uint32_t, uint32_t> get_position() override {
        return impl->get_position();
    }

    virtual std::unique_ptr<InputHandleBase> get_input() override {
        return std::make_unique<InputHandleWrapper<typename WindowImpl::InputType>>(
            impl->get_input()
        );
    }

    virtual void swap_buffers() override {
        impl->swap_buffers();
    }

    virtual void maximize() override {
        impl->maximize();
    }

    virtual void center() override {
        impl->center();
    }

    virtual void set_event_callback(const std::function<void(const Event&)>& callback) override {
        impl->set_event_callback(callback);
    }

private:
    std::unique_ptr<WindowImpl> impl;
};

/*
WindowFactoryWrapper:
Wrapper for a window factory based on external polymorphism. The window factory 
is responsible for creating new windows.
*/
template <WindowFactoryConcept FactoryImpl>
class WindowFactoryWrapper final : public WindowFactoryBase {
public:
    WindowFactoryWrapper(const FactoryImpl& impl_) : impl(impl_) {}

    virtual std::shared_ptr<WindowBase> create_window() override {
        return std::make_shared<WindowWrapper<typename FactoryImpl::WindowType>>(
            impl.create_window()
        );
    }

private:
    FactoryImpl impl;
};

private:
    std::unique_ptr<WindowFactoryBase> factory;

    // TODO: Replace with backend loader.
    BackendLoader loader{[](){ return false; }};
    BackendObserver observer{[](){ return false; }};
    BackendUnloader unloader{[](){ return false; }};
};

} // namespace pine
