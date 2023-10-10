#include "pine/platform/opengl/shader.hpp"

#include <glad/glad.h>

#include "pine/pch.hpp"
#include "pine/utils/filesystem.hpp"
#include "pine/utils/math.hpp"

#include "pine/platform/opengl/utilities.hpp"

namespace pine {

// ----------------------------------------------------------------------------
// OpenGLShader
// ----------------------------------------------------------------------------

OpenGLShader::OpenGLShader(const std::string& name, const RendererID id)
    : shader_name(name),
      renderer_id(id) {}

OpenGLShader::~OpenGLShader() { glDeleteProgram(renderer_id); }

void OpenGLShader::bind() const { glUseProgram(renderer_id); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::set_int(const std::string& name, const int value) const {
    upload_uniform_int(name, value);
}

void OpenGLShader::set_int_array(const std::string& name,
    const int* values,
    const uint32_t count) const {
    upload_uniform_int_array(name, const_cast<int*>(values), count);
}

void OpenGLShader::set_float(const std::string& name, const float value) const {
    upload_uniform_float(name, value);
}

void OpenGLShader::set_float3(const std::string& name,
    const Vec3& value) const {
    upload_uniform_float3(name, value);
}

void OpenGLShader::set_float4(const std::string& name,
    const Vec4& value) const {
    upload_uniform_float4(name, value);
}

void OpenGLShader::set_mat4(const std::string& name, const Mat4& value) const {
    upload_uniform_mat4(name, value);
}

void OpenGLShader::upload_uniform_int(const std::string& name,
    const int value) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_int_array(const std::string& name,
    const int* values,
    const uint32_t count) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform1iv(location, static_cast<GLsizei>(count), values);
}

void OpenGLShader::upload_uniform_float(const std::string& name,
    const float value) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float2(const std::string& name,
    const Vec2& values) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::upload_uniform_float3(const std::string& name,
    const Vec3& values) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::upload_uniform_float4(const std::string& name,
    const Vec4& values) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::upload_uniform_mat3(const std::string& name,
    const Mat3& matrix) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
}

void OpenGLShader::upload_uniform_mat4(const std::string& name,
    const Mat4& matrix) const {
    const auto location = glGetUniformLocation(renderer_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
}

// ----------------------------------------------------------------------------
// Factory methods
// ----------------------------------------------------------------------------

namespace opengl {

std::unique_ptr<OpenGLShader> create_shader(
    const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file) {
    // Check that files exist
    [[maybe_unused]] const auto vertex_exists =
        pine::filesystem::is_file(vertex_file);
    [[maybe_unused]] const auto fragment_exists =
        pine::filesystem::is_file(fragment_file);

    PINE_CORE_ASSERT(vertex_exists, "Vertex shader file does not exist.");
    PINE_CORE_ASSERT(fragment_exists, "Fragment shader file does not exist.");

    const auto name = vertex_file.stem().string();

    // Read sources from file
    const auto vertex_source = pine::filesystem::read_file_source(vertex_file);
    const auto fragment_source =
        pine::filesystem::read_file_source(fragment_file);

    const auto program =
        opengl::compile_shader(vertex_source, fragment_source);
    return std::make_unique<OpenGLShader>(name, program);
}

std::unique_ptr<OpenGLShader> create_shader(const std::string& name,
    const std::string& vertex_source,
    const std::string& fragment_source) {
    const auto program =
        opengl::compile_shader(vertex_source, fragment_source);
    return std::make_unique<OpenGLShader>(name, program);
}

} // namespace opengl

} // namespace pine
