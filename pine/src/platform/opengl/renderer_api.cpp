#include "pine/platform/opengl/renderer_api.hpp"

#include <glad/glad.h>

#include "pine/pch.hpp"

namespace pine
{

void OpenGLRendererAPI::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(const uint32_t x, const uint32_t y,
    const uint32_t width, const uint32_t height)
{
    glViewport(static_cast<GLint>(x),
        static_cast<GLint>(y),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height));
}

void OpenGLRendererAPI::SetClearColor(const Vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const VertexArray& vertex_array,
    const uint32_t index_count)
{
    const uint32_t count =
        index_count ? vertex_array.get_index_buffer().get_count() : index_count;
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES,
        static_cast<GLsizei>(count),
        GL_UNSIGNED_INT,
        nullptr);
}

} // namespace pine
