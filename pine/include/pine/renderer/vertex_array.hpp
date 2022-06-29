#pragma once

#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/buffer.hpp"

namespace pine
{

class VertexArray
{
public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetVertexBuffer(std::unique_ptr<VertexBuffer> buffer) = 0;
    virtual void SetIndexBuffer(std::unique_ptr<IndexBuffer> buffer) = 0;

    virtual VertexBuffer& GetVertexBuffer() const = 0;
    virtual IndexBuffer& GetIndexBuffer() const = 0;

    static std::unique_ptr<VertexArray> Create();
};

} // namespace pine
