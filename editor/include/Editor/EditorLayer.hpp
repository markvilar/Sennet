#pragma once

#include <memory>

#include "Pine/Pine.hpp"

namespace Pine
{

struct InterfaceLayout
{
    Vec2 Position = {0.0f, 0.0f};
    Vec2 Size = {0.0f, 0.0f};

    InterfaceLayout() = default;
    InterfaceLayout(const InterfaceLayout&) = default;
    InterfaceLayout(InterfaceLayout&&) = default;

    InterfaceLayout(const Vec2& position, const Vec2& size)
        : Position(position), Size(size)
    {
    }

    ~InterfaceLayout() = default;

    InterfaceLayout& operator=(const InterfaceLayout&) = default;
    InterfaceLayout& operator=(InterfaceLayout&&) = default;
};

class EditorServer : public TCP::Server
{
public:
    EditorServer(const uint16_t port) : TCP::Server(port) {}
    ~EditorServer() = default;

protected:
    bool OnClientConnect(
        const std::shared_ptr<TCP::Connection>& client) override
    {
        PINE_INFO("Client connected.", client);
        return true;
    }

    void OnClientDisconnect(
        const std::shared_ptr<TCP::Connection>& client) override

    {
        PINE_INFO("Client disconnected.");
    }

    void OnMessage(const std::shared_ptr<TCP::Connection>& client,
        const Message& message) override
    {
        PINE_INFO("Client {0} sent {1} bytes.", client->GetID(), 
            message.Header.Size);

        const auto text = [&message](){
            return std::string(message.Body.data(), 
                message.Body.data() + message.Body.size());
            }();

        PINE_INFO("Message: {0}", text.substr(0, 50));
    }
};

class EditorLayer : public Layer
{
public:
    EditorLayer();
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;

    virtual void OnImGuiRender() override;

    virtual void OnEvent(Event& e) override;

private:
    void UpdateInterfaceLayout();

private:
    OrthographicCameraController m_CameraController{1.0f};
    ShaderLibrary m_ShaderLibrary{};

    Image m_Image;

    TCP::Client m_Client{};
    EditorServer m_Server{60000};

    std::shared_ptr<Framebuffer> m_ViewportFramebuffer;
    std::shared_ptr<Texture2D> m_Texture;

    bool m_ViewportFocused{false};
    bool m_ViewportHovered{false};

    Renderer2D::RendererData m_RendererData2D{};

    Vec4 m_QuadColor{0.2f, 0.3f, 0.8f, 1.0f};
    float m_QuadRotation{0.0f};

    std::unordered_map<std::string, InterfaceLayout> m_InterfaceLayouts{};
};

} // namespace Pine
