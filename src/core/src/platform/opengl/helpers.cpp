#include "pine/platform/opengl/helpers.hpp"

#include "pine/core/assert.hpp"
#include "pine/pch.hpp"

namespace pine::opengl
{
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

} // namespace pine::opengl
