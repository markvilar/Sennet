#pragma once

#include <memory>

#include "Pine/Core/Base.hpp"
#include "Pine/Renderer/Buffer.hpp"

namespace Pine
{

class VertexArray
{
public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(
        const std::shared_ptr<VertexBuffer>& buffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>&
    GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

    static std::unique_ptr<VertexArray> Create();
};

} // namespace Pine
