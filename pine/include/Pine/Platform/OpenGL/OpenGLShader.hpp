#pragma once

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
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc,
        const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const std::string& name, const int value) override;
    virtual void SetIntArray(const std::string& name, const int* values,
        const uint32_t count) override;
    virtual void SetFloat(const std::string& name, const float value) override;
    virtual void SetFloat3(const std::string& name, const Vec3& value) override;
    virtual void SetFloat4(const std::string& name, const Vec4& value) override;
    virtual void SetMat4(const std::string& name, const Mat4& value) override;

    virtual const std::string& GetName() const override { return m_Name; }

    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformIntArray(
        const std::string& name, int* values, uint32_t count);
    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const Vec2& values);
    void UploadUniformFloat3(const std::string& name, const Vec3& values);
    void UploadUniformFloat4(const std::string& name, const Vec4& values);

    void UploadUniformMat3(const std::string& name, const Mat3& matrix);
    void UploadUniformMat4(const std::string& name, const Mat4& matrix);

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(
        const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    RendererID m_RendererID;
    std::string m_Name;
};

} // namespace Pine
