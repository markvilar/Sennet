#include "Pine/Platform/OpenGL/OpenGLVertexArray.hpp"

#include <glad/glad.h>

#include "Pine/Pch.hpp"

namespace Pine
{

static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Uint:
        return GL_UNSIGNED_INT;
    case ShaderDataType::Uint2:
        return GL_UNSIGNED_INT;
    case ShaderDataType::Uint3:
        return GL_UNSIGNED_INT;
    case ShaderDataType::Uint4:
        return GL_UNSIGNED_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    case ShaderDataType::None:
        return 0;
    }

    PINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::SetVertexBuffer(std::unique_ptr<VertexBuffer> buffer)
{
    glBindVertexArray(m_RendererID);
    buffer->Bind();

    PINE_CORE_ASSERT(buffer->GetLayout().GetElements().size(),
        "Vertex Buffer has no layout!")

    uint32_t index = 0;
    const auto& layout = buffer->GetLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            element.GetComponentCount(),
            ShaderDataTypeToOpenGLBaseType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<const void*>(element.Offset));
        index++;
    }

    m_VertexBuffer.reset(buffer.release());
}

void OpenGLVertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer> buffer)
{
    glBindVertexArray(m_RendererID);
    buffer->Bind();
    m_IndexBuffer.reset(buffer.release());
}

} // namespace Pine
