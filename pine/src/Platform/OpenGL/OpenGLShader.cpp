#include "Pine/Platform/OpenGL/OpenGLShader.hpp"

#include <fstream>

#include <glad/glad.h>

#include "Pine/Pch.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

static auto ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    PINE_CORE_ERROR("Could not convert shader type '{0}'", type);
    PINE_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    const auto source = ReadFile(filepath);
    const auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    // Extract name from filepath.
    const auto lastSlash = filepath.find_last_of("/\\");
    const auto lastSlashIndex =
        lastSlash == std::string::npos ? 0 : lastSlash + 1;
    const auto lastDot = filepath.rfind(".");
    const auto count = lastDot == std::string::npos
        ? filepath.size() - lastSlashIndex
        : lastDot - lastSlashIndex;
    m_Name = filepath.substr(lastSlashIndex, count);
}

OpenGLShader::OpenGLShader(const std::string& name,
    const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name)
{
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = vertexSrc;
    shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(shaderSources);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

std::string OpenGLShader::ReadFile(const std::string& filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        PINE_CORE_ERROR("Could not open file '{0}'", filepath);
    }

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(
    const std::string& source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    // TODO: Refine and clean up!
    constexpr auto typeToken = "#type";
    const auto typeTokenLength = strlen(typeToken);
    auto pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        const auto eol = source.find_first_of("\r\n", pos);
        PINE_CORE_ASSERT(eol != std::string::npos,
            "Syntax error in shader preprocessing!");
        const auto begin = pos + typeTokenLength + 1;
        const auto type = source.substr(begin, eol - begin);
        PINE_CORE_ASSERT(ShaderTypeFromString(type),
            "Invalid shader type specified!");

        const auto nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);

        const auto entrySize =
            nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos;
        shaderSources[ShaderTypeFromString(type)] =
            source.substr(nextLinePos, pos - entrySize);
    }

    return shaderSources;
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, std::string>& shaderSources)
{
    GLuint program = glCreateProgram();
    PINE_CORE_ASSERT(shaderSources.size() <= 2,
        "Pine only supports 2 shaders for now.");
    std::array<GLenum, 2> glShaderIDs;
    auto glShaderIDIndex = 0;
    for (auto& kv : shaderSources)
    {
        const auto& type = kv.first;
        const auto& source = kv.second;
        const auto shader = glCreateShader(type);
        const auto sourceCStr = source.c_str();

        glShaderSource(shader, 1, &sourceCStr, 0);
        glCompileShader(shader);

        auto isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            // TODO: Add shader type logging.
            PINE_CORE_ERROR("{0}", infoLog.data());
            PINE_CORE_ASSERT(false, "Shader compilation failure!")
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    glLinkProgram(program);

    auto isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        auto maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
        {
            glDeleteShader(id);
        }

        PINE_CORE_ERROR("{0}", infoLog.data());
        PINE_CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : glShaderIDs)
    {
        glDetachShader(program, id);
    }

    m_RendererID = program;
}

void OpenGLShader::Bind() const { glUseProgram(m_RendererID); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

void OpenGLShader::SetInt(const std::string& name, const int value) const
{
    UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(
    const std::string& name, const int* values, const uint32_t count) const
{
    UploadUniformIntArray(name, const_cast<int*>(values), count);
}

void OpenGLShader::SetFloat(const std::string& name, const float value) const
{
    UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const Vec3& value) const
{
    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const Vec4& value) const
{
    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const Mat4& value) const
{
    UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(
    const std::string& name, const int value) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(
    const std::string& name, const int* values, const uint32_t count) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(
    const std::string& name, const float value) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(
    const std::string& name, const Vec2& values) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(
    const std::string& name, const Vec3& values) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(
    const std::string& name, const Vec4& values) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(
    const std::string& name, const Mat3& matrix) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, ValuePtr(matrix));
}

void OpenGLShader::UploadUniformMat4(
    const std::string& name, const Mat4& matrix) const
{
    const auto location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, ValuePtr(matrix));
}

} // namespace Pine
