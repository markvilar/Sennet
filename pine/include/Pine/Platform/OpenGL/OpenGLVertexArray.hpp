#pragma once

#include <memory>

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

    virtual void SetVertexBuffer(std::unique_ptr<VertexBuffer> buffer) override;
    virtual void SetIndexBuffer(std::unique_ptr<IndexBuffer> buffer) override;

    virtual VertexBuffer& GetVertexBuffer() const override
    {
        return *m_VertexBuffer.get();
    }

    virtual IndexBuffer& GetIndexBuffer() const override
    {
        return *m_IndexBuffer.get();
    }

private:
    RendererID m_RendererID = {};
    std::unique_ptr<VertexBuffer> m_VertexBuffer = {};
    std::unique_ptr<IndexBuffer> m_IndexBuffer = {};
};

} // namespace Pine
