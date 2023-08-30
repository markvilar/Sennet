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
    TODO: Create design based on bridge design pattern (i.e. pimpl).
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
concept WindowContextConcept = requires(T t) {
    typename T::WindowType;
    typename T::InputType;
    t.create_window();
};

class BasicInputHandle {
public:
    virtual ~BasicInputHandle() = default;
    virtual bool is_key_pressed(const KeyCode key) = 0;
    virtual bool is_mouse_button_pressed(const MouseCode button) = 0;
    virtual std::pair<float, float> get_mouse_position() = 0;
};

template <InputConcept Impl>
class ErasedInputHandle final : public BasicInputHandle {
public:
    ErasedInputHandle(Impl impl_) : impl(std::move(impl_)) {}
    virtual ~ErasedInputHandle() = default;
    
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
    Impl impl;
};

class BasicWindow {
public:
    using size_t = std::uint32_t;

    virtual ~BasicWindow() = default;
    virtual std::string_view get_title() = 0;
    virtual std::pair<size_t, size_t> get_size() = 0;
    virtual std::pair<size_t, size_t> get_position() = 0;
    virtual std::unique_ptr<BasicInputHandle> get_input() = 0;
    virtual void swap_buffers() = 0;
    virtual void maximize() = 0;
    virtual void center() = 0;
    virtual void set_event_callback(const std::function<void(const Event&)>&) = 0;
};

/*
WindowContext:
Context for loading of window backend, and creation of windows. The client is 
responsible for loading and unloading the window backend, and maintaining the 
lifetime of the created windows.

Public interfaces:
- BasicWindowContext

Private implementation:
- ErasedWindow
- WindowContextWrapper
*/

class WindowContext {

public:

class BasicWindowContext {
public:
    virtual ~BasicWindowContext() = default;
    virtual std::shared_ptr<BasicWindow> create_window() = 0;
};

private:

template <WindowConcept Impl>
class ErasedWindow final : public BasicWindow {
public:
    ErasedWindow(std::unique_ptr<Impl> impl_) : impl(std::move(impl_)) {}

    virtual std::string_view get_title() override {
        return impl->get_title();
    }
    
    virtual std::pair<uint32_t, uint32_t> get_size() override {
        return impl->get_size();
    }
    
    virtual std::pair<uint32_t, uint32_t> get_position() override {
        return impl->get_position();
    }

    virtual std::unique_ptr<BasicInputHandle> get_input() override {
        return std::make_unique<ErasedInputHandle<typename Impl::InputType>>(impl->get_input());
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
    std::unique_ptr<Impl> impl;
};

template <WindowContextConcept Impl>
class WindowContextWrapper final : public BasicWindowContext {
public:
    WindowContextWrapper(const Impl& impl_) : impl(impl_) {}

    virtual std::shared_ptr<BasicWindow> create_window() override {
        return std::make_shared<ErasedWindow<typename Impl::WindowType>>(impl.create_window());
    }

private:
    Impl impl;
};

public:
    using BackendLoader = std::function<bool(void)>;
    using BackendVerifier = std::function<bool(void)>;
    using BackendUnloader = std::function<bool(void)>;

    template <WindowContextConcept Impl>
    WindowContext(Impl impl_, 
        const BackendLoader& loader_,
        const BackendVerifier& verifier_,
        const BackendUnloader& unloader_
    ) 
    : impl(std::make_shared<WindowContextWrapper<Impl>>(std::move(impl_))),
        loader(loader_), verifier(verifier_), unloader(unloader_) {}

    bool load() const noexcept { return loader(); }
    bool is_loaded() const noexcept { return verifier(); }
    bool unload() const noexcept { return unloader(); }

    /* Builder */
    std::shared_ptr<BasicWindow> create_window() {
        return impl->create_window();
    }

private:
    std::shared_ptr<BasicWindowContext> impl;
    BackendLoader loader{[](){ return false; }};
    BackendVerifier verifier{[](){ return false; }};
    BackendUnloader unloader{[](){ return false; }};
};

} // namespace pine
