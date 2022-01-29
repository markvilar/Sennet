#include "Pine/Renderer/Buffer.hpp"
#include "Pine/Pch.hpp"

#include "Pine/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Pine/Renderer/Renderer.hpp"
#include "Pine/Renderer/RendererAPI.hpp"

namespace Pine
{

void BufferLayout::CalculateOffsetAndStride()
{
    uint32_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements)
    {
        element.Offset = offset;
        offset += element.Size;
        m_Stride += element.Size;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    PINE_CORE_ASSERT(false, "Unknown renderer API.");
    return nullptr;
}

} // namespace Pine
