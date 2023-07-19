#pragma once

#include "pine/renderer/buffer.hpp"
#include "pine/renderer/renderer_api.hpp"

namespace pine {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const uint32_t size);
    OpenGLVertexBuffer(const float* vertices, const uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void set_data(const void* data, const uint32_t size) override;

    virtual const VertexBufferLayout& get_layout() const override {
        return layout;
    }
    virtual void set_layout(const VertexBufferLayout& layout_) override {
        layout = layout_;
    }

private:
    RendererID renderer_id;
    VertexBufferLayout layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t get_count() const override { return count; }

private:
    RendererID renderer_id;
    uint32_t count;
};

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void set_vertex_buffer(
        std::unique_ptr<VertexBuffer> buffer) override;
    virtual void set_index_buffer(std::unique_ptr<IndexBuffer> buffer) override;

    virtual VertexBuffer& get_vertex_buffer() const override {
        return *vertex_buffer.get();
    }

    virtual IndexBuffer& get_index_buffer() const override {
        return *index_buffer.get();
    }

private:
    RendererID renderer_id = {};
    std::unique_ptr<VertexBuffer> vertex_buffer = {};
    std::unique_ptr<IndexBuffer> index_buffer = {};
};

} // namespace pine
