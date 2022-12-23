#include "pine/platform/opengl/shader.hpp"

#include <fstream>

#include <glad/glad.h>

#include "pine/pch.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

static auto to_opengl_shader_type(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    PINE_CORE_ERROR("Could not convert shader type '{0}'", type);
    PINE_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

OpenGLShader::OpenGLShader(const std::filesystem::path& filepath)
{
    const auto source = read_file(filepath);
    const auto shader_sources = preprocess(source);
    compile_shader(shader_sources);
    m_name = filepath.stem();
}

OpenGLShader::OpenGLShader(const std::string& name,
    const std::string& vertex_source, const std::string& fragment_source)
    : m_name(name)
{
    std::unordered_map<GLenum, std::string> shader_sources;
    shader_sources[GL_VERTEX_SHADER] = vertex_source;
    shader_sources[GL_FRAGMENT_SHADER] = fragment_source;
    compile_shader(shader_sources);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_renderer_id); }

std::string OpenGLShader::read_file(const std::filesystem::path& filepath)
{
    std::string result;
    std::ifstream input_stream(filepath, std::ios::in | std::ios::binary);
    if (input_stream)
    {
        input_stream.seekg(0, std::ios::end);
        result.resize(static_cast<uint32_t>(input_stream.tellg()));
        input_stream.seekg(0, std::ios::beg);
        input_stream.read(&result[0], static_cast<int>(result.size()));
        input_stream.close();
    }
    else
    {
        PINE_CORE_ERROR("Could not open file '{0}'", filepath);
    }

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(
    const std::string& source)
{
    std::unordered_map<GLenum, std::string> shader_sources;

    // TODO: Refine and clean up!
    constexpr auto type_token = "#type";
    const auto type_token_length = strlen(type_token);
    auto pos = source.find(type_token, 0);
    while (pos != std::string::npos)
    {
        const auto eol = source.find_first_of("\r\n", pos);
        PINE_CORE_ASSERT(eol != std::string::npos,
            "Syntax error in shader preprocessing!");
        const auto begin = pos + type_token_length + 1;
        const auto type = source.substr(begin, eol - begin);
        PINE_CORE_ASSERT(to_opengl_shader_type(type),
            "Invalid shader type specified!");

        const auto next_line_pos = source.find_first_not_of("\r\n", eol);
        pos = source.find(type_token, next_line_pos);

        const auto entry_size = next_line_pos == std::string::npos
            ? source.size() - 1
            : next_line_pos;
        shader_sources[static_cast<uint32_t>(to_opengl_shader_type(type))] =
            source.substr(next_line_pos, pos - entry_size);
    }

    return shader_sources;
}

void OpenGLShader::compile_shader(
    const std::unordered_map<GLenum, std::string>& shader_sources)
{
    const auto program = glCreateProgram();
    PINE_CORE_ASSERT(shader_sources.size() <= 2,
        "pine only supports 2 shaders for now.");
    std::array<GLenum, 2> shader_ids;
    auto shader_id_index = 0;
    for (auto& source : shader_sources)
    {
        const auto& source_type = source.first;
        const auto source_string = source.second.c_str();
        const auto shader = glCreateShader(source_type);

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

            // TODO: Add shader type logging.
            PINE_CORE_ERROR("{0}", info_log.data());
            PINE_CORE_ASSERT(false, "Shader compilation failure!")
            break;
        }

        glAttachShader(program, shader);
        shader_ids[static_cast<uint32_t>(shader_id_index++)] = shader;
    }

    glLinkProgram(program);

    auto is_linked = 0;
    // NOTE: Was const_cast<int*>(&is_linked)
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE)
    {
        auto max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        // The max_length includes the NULL character
        std::vector<GLchar> info_log(static_cast<uint32_t>(max_length));
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : shader_ids)
        {
            glDeleteShader(id);
        }

        PINE_CORE_ERROR("{0}", info_log.data());
        PINE_CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : shader_ids)
    {
        glDetachShader(program, id);
    }

    m_renderer_id = program;
}

void OpenGLShader::bind() const { glUseProgram(m_renderer_id); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::set_int(const std::string& name, const int value) const
{
    upload_uniform_int(name, value);
}

void OpenGLShader::set_int_array(const std::string& name, const int* values,
    const uint32_t count) const
{
    upload_uniform_int_array(name, const_cast<int*>(values), count);
}

void OpenGLShader::set_float(const std::string& name, const float value) const
{
    upload_uniform_float(name, value);
}

void OpenGLShader::set_float3(const std::string& name, const Vec3& value) const
{
    upload_uniform_float3(name, value);
}

void OpenGLShader::set_float4(const std::string& name, const Vec4& value) const
{
    upload_uniform_float4(name, value);
}

void OpenGLShader::set_mat4(const std::string& name, const Mat4& value) const
{
    upload_uniform_mat4(name, value);
}

void OpenGLShader::upload_uniform_int(const std::string& name,
    const int value) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_int_array(const std::string& name,
    const int* values, const uint32_t count) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1iv(location, static_cast<GLsizei>(count), values);
}

void OpenGLShader::upload_uniform_float(const std::string& name,
    const float value) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float2(const std::string& name,
    const Vec2& values) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::upload_uniform_float3(const std::string& name,
    const Vec3& values) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::upload_uniform_float4(const std::string& name,
    const Vec4& values) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::upload_uniform_mat3(const std::string& name,
    const Mat3& matrix) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
}

void OpenGLShader::upload_uniform_mat4(const std::string& name,
    const Mat4& matrix) const
{
    const auto location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
}

} // namespace pine
