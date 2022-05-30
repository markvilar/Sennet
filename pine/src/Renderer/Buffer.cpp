#include "Pine/Renderer/Buffer.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Pine/Renderer/Renderer.hpp"
#include "Pine/Renderer/RendererAPI.hpp"

namespace Pine
{

void BufferLayout::CalculateOffsetAndStride()
{
    m_Stride = [this]()
    {
        uint32_t offset = 0;
        uint32_t stride = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            stride += element.Size;
        }
        return stride;
    }();
}

std::unique_ptr<VertexBuffer> VertexBuffer::Create(const uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

std::unique_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
    const uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(vertices, size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

std::unique_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices,
    const uint32_t count)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLIndexBuffer>(indices, count);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

} // namespace Pine
