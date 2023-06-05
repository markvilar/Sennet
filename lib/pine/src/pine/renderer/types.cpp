#include "pine/renderer/types.hpp"

#include "pine/defines/assert.hpp"

namespace pine
{

// ----------------------------------------------------------------------------
// TextureFormat
// ----------------------------------------------------------------------------

uint32_t get_byte_size(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RED:
        return 1;
    case TextureFormat::RG:
        return 1;
    case TextureFormat::RGB:
        return 1;
    case TextureFormat::BGR:
        return 1;
    case TextureFormat::RGBA:
        return 1;
    case TextureFormat::BGRA:
        return 1;
    default:
        return 0;
    }
}

uint32_t get_components(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RED:
        return 1;
    case TextureFormat::RG:
        return 2;
    case TextureFormat::RGB:
        return 3;
    case TextureFormat::BGR:
        return 3;
    case TextureFormat::RGBA:
        return 4;
    case TextureFormat::BGRA:
        return 4;
    default:
        return 0;
    }
}

bool is_color_format(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::INVALID:
        return false;
    case TextureFormat::RED:
        return true;
    case TextureFormat::RG:
        return true;
    case TextureFormat::RGB:
        return true;
    case TextureFormat::BGR:
        return true;
    case TextureFormat::RGBA:
        return true;
    case TextureFormat::BGRA:
        return true;
    case TextureFormat::DEPTH32F:
        return false;
    case TextureFormat::DEPTH24STENCIL8:
        return false;
    default:
        return false;
    }
    return false;
}

bool is_depth_format(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::INVALID:
        return false;
    case TextureFormat::RED:
        return false;
    case TextureFormat::RG:
        return false;
    case TextureFormat::RGB:
        return false;
    case TextureFormat::BGR:
        return false;
    case TextureFormat::RGBA:
        return false;
    case TextureFormat::BGRA:
        return false;
    case TextureFormat::DEPTH32F:
        return true;
    case TextureFormat::DEPTH24STENCIL8:
        return true;
    default:
        return false;
    }
    return false;
}

// ----------------------------------------------------------------------------
// ShaderDataType
// ----------------------------------------------------------------------------

uint32_t get_byte_size(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::NONE:
        return 0;
    case ShaderDataType::FLOAT:
        return 4;
    case ShaderDataType::FLOAT2:
        return 4 * 2;
    case ShaderDataType::FLOAT3:
        return 4 * 3;
    case ShaderDataType::FLOAT4:
        return 4 * 4;
    case ShaderDataType::MAT3:
        return 4 * 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4 * 4;
    case ShaderDataType::INT:
        return 4;
    case ShaderDataType::INT2:
        return 4 * 2;
    case ShaderDataType::INT3:
        return 4 * 3;
    case ShaderDataType::INT4:
        return 4 * 4;
    case ShaderDataType::UINT:
        return 4;
    case ShaderDataType::UINT2:
        return 4 * 2;
    case ShaderDataType::UINT3:
        return 4 * 3;
    case ShaderDataType::UINT4:
        return 4 * 4;
    case ShaderDataType::BOOL:
        return 1;
    }

    PINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

uint32_t get_components(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::NONE:
        return 0;
    case ShaderDataType::FLOAT:
        return 1;
    case ShaderDataType::FLOAT2:
        return 2;
    case ShaderDataType::FLOAT3:
        return 3;
    case ShaderDataType::FLOAT4:
        return 4;
    case ShaderDataType::MAT3:
        return 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4;
    case ShaderDataType::INT:
        return 1;
    case ShaderDataType::INT2:
        return 2;
    case ShaderDataType::INT3:
        return 3;
    case ShaderDataType::INT4:
        return 4;
    case ShaderDataType::UINT:
        return 1;
    case ShaderDataType::UINT2:
        return 2;
    case ShaderDataType::UINT3:
        return 3;
    case ShaderDataType::UINT4:
        return 4;
    case ShaderDataType::BOOL:
        return 1;
    }

    PINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

bool is_integer_type(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::NONE:
        return false;
    case ShaderDataType::FLOAT:
        return false;
    case ShaderDataType::FLOAT2:
        return false;
    case ShaderDataType::FLOAT3:
        return false;
    case ShaderDataType::FLOAT4:
        return false;
    case ShaderDataType::MAT3:
        return false;
    case ShaderDataType::MAT4:
        return false;
    case ShaderDataType::INT:
        return true;
    case ShaderDataType::INT2:
        return true;
    case ShaderDataType::INT3:
        return true;
    case ShaderDataType::INT4:
        return true;
    case ShaderDataType::UINT:
        return true;
    case ShaderDataType::UINT2:
        return true;
    case ShaderDataType::UINT3:
        return true;
    case ShaderDataType::UINT4:
        return true;
    case ShaderDataType::BOOL:
        return false;
    }
    PINE_CORE_ASSERT(false, "Unknown shader data type.");
    return 0;
}

bool is_float_type(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::NONE:
        return false;
    case ShaderDataType::FLOAT:
        return true;
    case ShaderDataType::FLOAT2:
        return true;
    case ShaderDataType::FLOAT3:
        return true;
    case ShaderDataType::FLOAT4:
        return true;
    case ShaderDataType::MAT3:
        return true;
    case ShaderDataType::MAT4:
        return true;
    case ShaderDataType::INT:
        return false;
    case ShaderDataType::INT2:
        return false;
    case ShaderDataType::INT3:
        return false;
    case ShaderDataType::INT4:
        return false;
    case ShaderDataType::UINT:
        return false;
    case ShaderDataType::UINT2:
        return false;
    case ShaderDataType::UINT3:
        return false;
    case ShaderDataType::UINT4:
        return false;
    case ShaderDataType::BOOL:
        return false;
    }
    PINE_CORE_ASSERT(false, "Unknown shader data type.");
    return 0;
}

} // namespace pine
