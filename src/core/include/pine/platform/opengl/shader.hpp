#pragma once

#include <filesystem>
#include <string>

#include "pine/renderer/renderer_api.hpp"
#include "pine/renderer/shader.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& shader_name, const RendererID id);
    virtual ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&&) = delete;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&&) = delete;

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void set_int(const std::string& name,
        const int value) const override;
    virtual void set_int_array(const std::string& name,
        const int* values,
        const uint32_t count) const override;
    virtual void set_float(const std::string& name,
        const float value) const override;
    virtual void set_float3(const std::string& name,
        const Vec3& value) const override;
    virtual void set_float4(const std::string& name,
        const Vec4& value) const override;
    virtual void set_mat4(const std::string& name,
        const Mat4& value) const override;

    virtual const std::string& get_name() const override { return shader_name; }

    void upload_uniform_int(const std::string& name, const int value) const;
    void upload_uniform_int_array(const std::string& name,
        const int* values,
        const uint32_t count) const;

    void upload_uniform_float(const std::string& name, const float value) const;
    void upload_uniform_float2(const std::string& name,
        const Vec2& values) const;
    void upload_uniform_float3(const std::string& name,
        const Vec3& values) const;
    void upload_uniform_float4(const std::string& name,
        const Vec4& values) const;

    void upload_uniform_mat3(const std::string& name, const Mat3& matrix) const;
    void upload_uniform_mat4(const std::string& name, const Mat4& matrix) const;

private:
    RendererID renderer_id;
    std::string shader_name;
};

namespace opengl
{

// Create from files
std::unique_ptr<OpenGLShader> create_shader(
    const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file);

// Create from source
std::unique_ptr<OpenGLShader> create_shader(const std::string& name,
    const std::string& vertex_source,
    const std::string& fragment_source);

} // namespace opengl

} // namespace pine
