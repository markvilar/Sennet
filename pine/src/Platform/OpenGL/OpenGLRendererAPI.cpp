#include "Pine/Platform/OpenGL/OpenGLRendererAPI.hpp"

#include <glad/glad.h>

#include "Pine/Pch.hpp"

namespace Pine
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
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const Vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(
    const std::shared_ptr<VertexArray>& vertexArray, const uint32_t indexCount)
{
    const uint32_t count =
        indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace Pine
