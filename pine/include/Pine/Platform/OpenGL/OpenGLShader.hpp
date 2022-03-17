#pragma once

#include <filesystem>
#include <string>

#include "Pine/Renderer/RendererAPI.hpp"
#include "Pine/Renderer/Shader.hpp"
#include "Pine/Utils/Math.hpp"

// TODO: Remove!
typedef unsigned int GLenum;

namespace Pine
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::filesystem::path& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc,
        const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&&) = default;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&&) = default;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(
        const std::string& name, const int value) const override;
    virtual void SetIntArray(const std::string& name, const int* values,
        const uint32_t count) const override;
    virtual void SetFloat(
        const std::string& name, const float value) const override;
    virtual void SetFloat3(
        const std::string& name, const Vec3& value) const override;
    virtual void SetFloat4(
        const std::string& name, const Vec4& value) const override;
    virtual void SetMat4(
        const std::string& name, const Mat4& value) const override;

    virtual const std::string& GetName() const override { return m_Name; }

    void UploadUniformInt(const std::string& name, const int value) const;
    void UploadUniformIntArray(
        const std::string& name, const int* values, const uint32_t count) const;

    void UploadUniformFloat(const std::string& name, const float value) const;
    void UploadUniformFloat2(const std::string& name, const Vec2& values) const;
    void UploadUniformFloat3(const std::string& name, const Vec3& values) const;
    void UploadUniformFloat4(const std::string& name, const Vec4& values) const;

    void UploadUniformMat3(const std::string& name, const Mat3& matrix) const;
    void UploadUniformMat4(const std::string& name, const Mat4& matrix) const;

private:
    std::string ReadFile(const std::filesystem::path& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(
        const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    RendererID m_RendererID;
    std::string m_Name;
};

} // namespace Pine
