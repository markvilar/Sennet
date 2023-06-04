#include "buffer.hpp"

#include "renderer.hpp"
#include "renderer_api.hpp"

#include "pine/pch.hpp"

#include "../platform/opengl/buffer.hpp"

namespace pine
{

void VertexBufferLayout::calculate_offset_and_stride()
{
    stride = [this]()
    {
        uint32_t offset = 0;
        uint32_t stride = 0;
        for (auto& element : elements)
        {
            element.offset = offset;
            offset += element.size;
            stride += element.size;
        }
        return stride;
    }();
}

std::unique_ptr<VertexBuffer> VertexBuffer::create(const uint32_t size)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

std::unique_ptr<VertexBuffer> VertexBuffer::create(const float* vertices,
    const uint32_t size)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(vertices, size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

std::unique_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices,
    const uint32_t count)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLIndexBuffer>(indices, count);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

std::unique_ptr<VertexArray> VertexArray::create()
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexArray>();
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace pine
