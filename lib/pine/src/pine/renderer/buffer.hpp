#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pine/defines/common.hpp"
#include "pine/renderer/types.hpp"

namespace pine
{

struct VertexBufferElement
{
    std::string name = "";
    ShaderDataType type = ShaderDataType::NONE;
    uint32_t size = 0;
    uint32_t component_count = 0;
    uint32_t offset = 0;
    bool normalized = false;

    VertexBufferElement(const std::string& vertex_name,
        const ShaderDataType data_type,
        const bool is_data_normalized = false)
        : name(vertex_name),
          type(data_type),
          size(get_byte_size(data_type)),
          component_count(get_components(data_type)),
          offset(0),
          normalized(is_data_normalized)
    {
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() = default;

    VertexBufferLayout(const VertexBufferLayout&) = default;
    VertexBufferLayout(VertexBufferLayout&&) = default;

    VertexBufferLayout& operator=(const VertexBufferLayout&) = default;
    VertexBufferLayout& operator=(VertexBufferLayout&&) = default;

    ~VertexBufferLayout() = default;

    VertexBufferLayout(
        const std::initializer_list<VertexBufferElement>& elements_)
        : elements(elements_)
    {
        calculate_offset_and_stride();
    }

    inline uint32_t get_stride() const { return stride; }
    inline const std::vector<VertexBufferElement>& get_elements() const
    {
        return elements;
    }

    std::vector<VertexBufferElement>::iterator begin()
    {
        return elements.begin();
    }
    std::vector<VertexBufferElement>::iterator end() { return elements.end(); }
    std::vector<VertexBufferElement>::const_iterator begin() const
    {
        return elements.begin();
    }
    std::vector<VertexBufferElement>::const_iterator end() const
    {
        return elements.end();
    }

private:
    // TODO: Improvement - move calculations to factory.
    void calculate_offset_and_stride();

private:
    std::vector<VertexBufferElement> elements;
    uint32_t stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_data(const void* data, const uint32_t size) = 0;

    virtual const VertexBufferLayout& get_layout() const = 0;
    virtual void set_layout(const VertexBufferLayout& layout) = 0;

    static std::unique_ptr<VertexBuffer> create(const uint32_t size);
    static std::unique_ptr<VertexBuffer> create(const float* vertices,
        const uint32_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t get_count() const = 0;

    static std::unique_ptr<IndexBuffer> create(const uint32_t* indices,
        const uint32_t count);
};

class VertexArray
{
public:
    virtual ~VertexArray() {}

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_vertex_buffer(std::unique_ptr<VertexBuffer> buffer) = 0;
    virtual void set_index_buffer(std::unique_ptr<IndexBuffer> buffer) = 0;

    virtual VertexBuffer& get_vertex_buffer() const = 0;
    virtual IndexBuffer& get_index_buffer() const = 0;

    static std::unique_ptr<VertexArray> create();
};

} // namespace pine
