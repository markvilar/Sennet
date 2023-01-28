#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>

namespace pine::opengl
{
// Process shader program
GLuint process_shader(const GLenum type, const std::string& source);

// Compiles a shader and returns the renderer ID
GLuint compile_shader(const std::string& vertex_source,
    const std::string& fragment_source);

} // namespace pine::opengl
