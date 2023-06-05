#include "pine/platform/opengl/utilities.hpp"

#include "pine/pch.hpp"
#include "pine/core/log.hpp"
#include "pine/definitions/assert.hpp"

namespace pine::glutils
{

GLenum to_opengl(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::INVALID:
        return GL_INVALID_ENUM;
    case TextureFormat::RED:
        return GL_RED;
    case TextureFormat::RG:
        return GL_RG;
    case TextureFormat::RGB:
        return GL_RGB;
    case TextureFormat::BGR:
        return GL_BGR;
    case TextureFormat::RGBA:
        return GL_RGBA;
    case TextureFormat::BGRA:
        return GL_BGRA;
    case TextureFormat::DEPTH32F:
        return GL_DEPTH_COMPONENT32F;
    case TextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH24_STENCIL8;
    default:
        return GL_INVALID_ENUM;
    }
}

GLenum to_opengl_internal(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::INVALID:
        return GL_INVALID_ENUM;
    case TextureFormat::RED:
        return GL_R8;
    case TextureFormat::RG:
        return GL_RG8;
    case TextureFormat::RGB:
        return GL_RGB8;
    case TextureFormat::BGR:
        return GL_RGB8;
    case TextureFormat::RGBA:
        return GL_RGBA8;
    case TextureFormat::BGRA:
        return GL_RGBA8;
    case TextureFormat::DEPTH32F:
        return GL_DEPTH_COMPONENT;
    case TextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH_STENCIL;
    default:
        return GL_INVALID_ENUM;
    }
    return GL_INVALID_ENUM;
}

GLenum get_attachment_type(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::INVALID:
        return GL_INVALID_ENUM;
    case TextureFormat::RED:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::RG:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::RGB:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::BGR:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::RGBA:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::BGRA:
        return GL_COLOR_ATTACHMENT0;
    case TextureFormat::DEPTH32F:
        return GL_DEPTH_ATTACHMENT;
    case TextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH_STENCIL_ATTACHMENT;
    default:
        return GL_INVALID_ENUM;
    }
    return GL_INVALID_ENUM;
}

GLenum to_opengl(const TextureFilter filter)
{
    switch (filter)
    {
    case TextureFilter::NONE:
        return GL_INVALID_ENUM;
    case TextureFilter::NEAREST:
        return GL_NEAREST;
    case TextureFilter::LINEAR:
        return GL_LINEAR;
    default:
        return GL_INVALID_ENUM;
    }
}

GLenum to_opengl(const TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::NONE:
        return GL_INVALID_ENUM;
    case TextureWrap::CLAMP:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::REPEAT:
        return GL_REPEAT;
    default:
        return GL_INVALID_ENUM;
    }
}

GLenum to_opengl(const ImageFormat format)
{
    const auto data_format = [format]()
    {
        switch (format)
        {
        case ImageFormat::INVALID:
            return GL_INVALID_ENUM;
        case ImageFormat::GRAY:
            return GL_RED;
        case ImageFormat::GRAY_ALPHA:
            return GL_RG;
        case ImageFormat::RGB:
            return GL_RGB;
        case ImageFormat::BGR:
            return GL_BGR;
        case ImageFormat::RGBA:
            return GL_RGBA;
        case ImageFormat::BGRA:
            return GL_BGRA;
        }
        return GL_INVALID_ENUM;
    }();

    PINE_CORE_ASSERT(data_format, "Invalid OpenGL data format.");
    return static_cast<GLenum>(data_format);
}

GLenum to_opengl_internal(const ImageFormat format)
{
    const auto internal_format = [format]()
    {
        switch (format)
        {
        case ImageFormat::INVALID:
            return GL_INVALID_ENUM;
        case ImageFormat::GRAY:
            return GL_R8;
        case ImageFormat::GRAY_ALPHA:
            return GL_RG8;
        case ImageFormat::RGB:
            return GL_RGB8;
        case ImageFormat::BGR:
            return GL_RGB8;
        case ImageFormat::RGBA:
            return GL_RGBA8;
        case ImageFormat::BGRA:
            return GL_RGBA8;
        default:
            return GL_INVALID_ENUM;
        }
        return GL_INVALID_ENUM;
    }();

    PINE_CORE_ASSERT(internal_format, "Invalid OpenGL internal data format.");
    return static_cast<GLenum>(internal_format);
}

GLenum to_opengl(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::FLOAT:
        return GL_FLOAT;
    case ShaderDataType::FLOAT2:
        return GL_FLOAT;
    case ShaderDataType::FLOAT3:
        return GL_FLOAT;
    case ShaderDataType::FLOAT4:
        return GL_FLOAT;
    case ShaderDataType::MAT3:
        return GL_FLOAT;
    case ShaderDataType::MAT4:
        return GL_FLOAT;
    case ShaderDataType::INT:
        return GL_INT;
    case ShaderDataType::INT2:
        return GL_INT;
    case ShaderDataType::INT3:
        return GL_INT;
    case ShaderDataType::INT4:
        return GL_INT;
    case ShaderDataType::UINT:
        return GL_UNSIGNED_INT;
    case ShaderDataType::UINT2:
        return GL_UNSIGNED_INT;
    case ShaderDataType::UINT3:
        return GL_UNSIGNED_INT;
    case ShaderDataType::UINT4:
        return GL_UNSIGNED_INT;
    case ShaderDataType::BOOL:
        return GL_BOOL;
    case ShaderDataType::NONE:
        return GL_INVALID_ENUM;
    default:
        return GL_INVALID_ENUM;
    }
    PINE_CORE_ASSERT(false, "Unknown shader data type.");
    return GL_INVALID_ENUM;
}

std::string to_string(const GLenum type)
{
    switch (type)
    {
    case GL_VERTEX_SHADER:
        return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "GL_FRAGMENT_SHADER";
    }

    return std::string();
}

GLuint process_shader(const GLenum type, const std::string& source)
{
    const auto shader = glCreateShader(type);

    const auto source_string = source.c_str();
    glShaderSource(shader, 1, &source_string, 0);
    glCompileShader(shader);

    auto is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        auto max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(static_cast<uint32_t>(max_length));
        glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

        glDeleteShader(shader);

        const auto type_string = to_string(type);
        PINE_CORE_ERROR("{0}, {1}", type_string, info_log.data());
        PINE_CORE_ASSERT(false, "Shader compilation failure!")
    }

    return shader;
}

GLuint compile_shader(const std::string& vertex_source,
    const std::string& fragment_source)
{
    // Create new shader program
    const auto program = glCreateProgram();

    // Compile vertex shader
    const auto vertex_id = process_shader(GL_VERTEX_SHADER, vertex_source);

    // Compile fragment shader
    const auto fragment_id
        = process_shader(GL_FRAGMENT_SHADER, fragment_source);

    glAttachShader(program, vertex_id);
    glAttachShader(program, fragment_id);

    glLinkProgram(program);

    auto is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE)
    {
        auto max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(static_cast<uint32_t>(max_length));
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        // Clean up dangling program and shaders
        glDeleteProgram(program);
        glDeleteShader(vertex_id);
        glDeleteShader(fragment_id);

        PINE_CORE_ERROR("{0}", info_log.data());
        PINE_CORE_ASSERT(false, "Shader link failure!");
        return 0;
    }

    glDetachShader(program, vertex_id);
    glDetachShader(program, fragment_id);

    return program;
}

} // namespace pine::glutils
