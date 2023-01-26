#pragma once

#include <cstdint>
#include <string>

namespace pine::opengl
{

// OpenGL types
typedef uint32_t GLuint;
typedef uint32_t GLenum;

// Process shader program
GLuint process_shader(const GLenum type, const std::string& source);

// Compiles a shader and returns the renderer ID
GLuint compile_shader(const std::string& vertex_source,
    const std::string& fragment_source);

} // namespace pine::opengl
