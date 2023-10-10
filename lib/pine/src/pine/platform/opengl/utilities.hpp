#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>

#include "pine/renderer/image.hpp"
#include "pine/renderer/types.hpp"

namespace pine::opengl {

/*
Vendor and version functions:
- get_opengl_vendor
- get_opengl_renderer
- get_opengl_version
*/
std::string get_opengl_vendor();
std::string get_opengl_renderer();
std::string get_opengl_version();

/*
Texture conversion functions:
- to_opengl
- to_opengl_internal
- get_attachment_type
*/
GLenum to_opengl(const TextureFormat format);
GLenum to_opengl_internal(const TextureFormat format);

// TODO: Add support for multiple color attachments
GLenum get_attachment_type(const TextureFormat format);

// Texture operations
GLenum to_opengl(const TextureFilter filter);
GLenum to_opengl(const TextureWrap wrap);

/*
Image format conversion functions:
- to_opengl
- to_opengl_internal
*/
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
