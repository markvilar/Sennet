#pragma once

namespace pine
{

enum class RendererPrimitives
{
    None,
    Triangles,
    Lines,
};

enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Uint,
    Uint2,
    Uint3,
    Uint4,
    Bool
};

static constexpr uint32_t get_size(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Uint:
        return 4;
    case ShaderDataType::Uint2:
        return 4 * 2;
    case ShaderDataType::Uint3:
        return 4 * 3;
    case ShaderDataType::Uint4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    case ShaderDataType::None:
        return 0;
    }

    PINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

static constexpr uint32_t get_count(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 1;
    case ShaderDataType::Float2:
        return 2;
    case ShaderDataType::Float3:
        return 3;
    case ShaderDataType::Float4:
        return 4;
    case ShaderDataType::Mat3:
        return 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4;
    case ShaderDataType::Int:
        return 1;
    case ShaderDataType::Int2:
        return 2;
    case ShaderDataType::Int3:
        return 3;
    case ShaderDataType::Int4:
        return 4;
    case ShaderDataType::Uint:
        return 1;
    case ShaderDataType::Uint2:
        return 2;
    case ShaderDataType::Uint3:
        return 3;
    case ShaderDataType::Uint4:
        return 4;
    case ShaderDataType::Bool:
        return 1;
    case ShaderDataType::None:
        return 0;
    }

    PINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

};
