#pragma once

#include <cstdint>

namespace pine
{

using RendererID = uint32_t;

enum class GeometryPrimitive
{
    POINTS,
    LINES,
    TRIANGLES,
    LINE_STRIP,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
};

enum class TextureFormat
{
    INVALID,

    // Color
    RED,
    RG,
    RGB,
    BGR,
    RGBA,
    BGRA,

    // Depth
    DEPTH32F,
};

uint32_t get_byte_size(const TextureFormat format);
uint32_t get_components(const TextureFormat format);

bool is_color_format(const TextureFormat format);
bool is_depth_format(const TextureFormat format);

enum class TextureFilter
{
    NONE,
    NEAREST,
    LINEAR
};

enum class TextureWrap
{
    NONE,
    CLAMP,
    REPEAT
};

enum class ShaderDataType
{
    NONE,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MAT3,
    MAT4,
    INT,
    INT2,
    INT3,
    INT4,
    UINT,
    UINT2,
    UINT3,
    UINT4,
    BOOL
};

uint32_t get_byte_size(const ShaderDataType type);
uint32_t get_components(const ShaderDataType type);

bool is_integer_type(const ShaderDataType type);
bool is_float_type(const ShaderDataType type);

} // namespace pine
