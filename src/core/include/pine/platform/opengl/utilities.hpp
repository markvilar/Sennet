#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>

#include "pine/renderer/image.hpp"
#include "pine/renderer/types.hpp"

namespace pine::opengl
{

// Texture types
GLenum to_opengl(const TextureFormat format);
GLenum to_opengl_internal(const TextureFormat format);

// Texture operations
GLenum to_opengl(const TextureFilter filter);
GLenum to_opengl(const TextureWrap wrap);

// Image types
GLenum to_opengl(const ImageFormat image_format);
GLenum to_opengl_internal(const ImageFormat image_format);

// Shader types
GLenum to_opengl(const ShaderDataType type);

// String representations
std::string to_string(const GLenum type);

// Process shader program
GLuint process_shader(const GLenum type, const std::string& source);

// Compiles a shader and returns the renderer ID
GLuint compile_shader(const std::string& vertex_source,
    const std::string& fragment_source);

} // namespace pine::opengl
