#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pine/core/common.hpp"
#include "pine/renderer/common.hpp"

namespace pine
{

struct BufferElement
{
    std::string Name = "";
    ShaderDataType Type = ShaderDataType::None;
    uint32_t Offset = 0;
    uint32_t Size = 0;
    bool Normalized = false;

    BufferElement(const ShaderDataType type, const std::string& name,
        const bool normalized = false)
        : Type(type), Name(name), Size(get_size(type)), Offset(0),
          Normalized(normalized)
    {
    }

    uint32_t GetComponentCount() const
    {
        return get_count(Type);
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(const BufferLayout&) = default;
    BufferLayout(BufferLayout&&) = default;

    BufferLayout& operator=(const BufferLayout&) = default;
    BufferLayout& operator=(BufferLayout&&) = default;

    ~BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetAndStride();
    }


    inline uint32_t GetStride() const { return m_Stride; }
    inline const std::vector<BufferElement>& GetElements() const
    {
        return m_Elements;
    }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const
    {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const
    {
        return m_Elements.end();
    }

private:
    void CalculateOffsetAndStride();

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, const uint32_t size) = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static std::unique_ptr<VertexBuffer> Create(const uint32_t size);
    static std::unique_ptr<VertexBuffer> Create(const float* vertices,
        const uint32_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static std::unique_ptr<IndexBuffer> Create(const uint32_t* indices,
        const uint32_t count);
};

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
