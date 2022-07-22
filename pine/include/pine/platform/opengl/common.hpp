#pragma once

#include <cstdint>

namespace pine {

using GLenum = uint32_t;

static constexpr GLenum to_opengl(const ShaderDataType type)
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
    PINE_CORE_ASSERT(false, "Unknown shader data type.");
    return 0;
}

}; // namespace pine
