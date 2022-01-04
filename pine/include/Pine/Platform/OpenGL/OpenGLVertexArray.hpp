#pragma once

#include "Pine/Renderer/RendererAPI.hpp"
#include "Pine/Renderer/VertexArray.hpp"

namespace Pine
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    virtual const std::vector<Ref<VertexBuffer>>&
    GetVertexBuffers() const override
    {
        return m_VertexBuffers;
    }

    virtual const Ref<IndexBuffer>& GetIndexBuffer() const override
    {
        return m_IndexBuffer;
    }

private:
    RendererID m_RendererID;
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};

} // namespace Pine
