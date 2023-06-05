#include "pine/platform/opengl/buffer.hpp"

#include <glad/glad.h>

#include "pine/pch.hpp"

#include "pine/platform/opengl/utilities.hpp"

namespace pine
{

// ----------------------------------------------------------------------------
// ---- Vertex buffer ---------------------------------------------------------
// ----------------------------------------------------------------------------

OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size)
{
    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
{
    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &renderer_id); }

void OpenGLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::set_data(const void* data, const uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

// ----------------------------------------------------------------------------
// ---- Index buffer ----------------------------------------------------------
// ----------------------------------------------------------------------------

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices,
    const uint32_t count_)
    : count(count_)
{
    glCreateBuffers(1, &renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(count_ * sizeof(uint32_t)),
        indices,
        GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &renderer_id); }

void OpenGLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}

void OpenGLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ----------------------------------------------------------------------------
// ---- Vertex array ----------------------------------------------------------
// ----------------------------------------------------------------------------

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &renderer_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &renderer_id);
}

void OpenGLVertexArray::bind() const { glBindVertexArray(renderer_id); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::set_vertex_buffer(std::unique_ptr<VertexBuffer> buffer)
{
    glBindVertexArray(renderer_id);
    buffer->bind();

    PINE_CORE_ASSERT(buffer->get_layout().get_elements().size(),
        "Vertex Buffer has no layout!")

    uint32_t index = 0;
    const auto& layout = buffer->get_layout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        if (is_integer_type(element.type))
        {
            glVertexAttribIPointer(index,
                static_cast<GLint>(element.component_count),
                glutils::to_opengl(element.type),
                static_cast<GLsizei>(layout.get_stride()),
                reinterpret_cast<const void*>(element.offset));
        }
        else
        {
            glVertexAttribPointer(index,
                static_cast<GLint>(element.component_count),
                glutils::to_opengl(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(layout.get_stride()),
                reinterpret_cast<const void*>(element.offset));
        }
        index++;
    }

    vertex_buffer.reset(buffer.release());
}

void OpenGLVertexArray::set_index_buffer(std::unique_ptr<IndexBuffer> buffer)
{
    glBindVertexArray(renderer_id);
    buffer->bind();
    index_buffer.reset(buffer.release());
}

} // namespace pine
