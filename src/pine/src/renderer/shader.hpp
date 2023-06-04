#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "../utils/math.hpp"

namespace pine
{

// Forward declaration
class Shader;

// Factory method - Create from separate files
std::unique_ptr<Shader> create_shader(const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file);

// Factory method - Create from source
std::unique_ptr<Shader> create_shader(const std::string& name,
    const std::string& vertex_source,
    const std::string& fragment_source);

class Shader
{
public:
    Shader() = default;
    virtual ~Shader() = default;

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_int(const std::string& name, const int value) const = 0;
    virtual void set_int_array(const std::string& name,
        const int* values,
        const uint32_t count) const
        = 0;
    virtual void set_float(const std::string& name, const float value) const
        = 0;
    virtual void set_float3(const std::string& name, const Vec3& value) const
        = 0;
    virtual void set_float4(const std::string& name, const Vec4& value) const
        = 0;
    virtual void set_mat4(const std::string& name, const Mat4& value) const = 0;

    virtual const std::string& get_name() const = 0;
};

class ShaderLibrary
{
    using ShaderMap = std::unordered_map<std::string, std::shared_ptr<Shader>>;

public:
    void add_shader(const std::string& name,
        const std::shared_ptr<Shader>& shader);
    void add_shader(const std::shared_ptr<Shader>& shader);

    bool load_shader(const std::filesystem::path& vertex_file,
        const std::filesystem::path& fragment_file);

    const std::shared_ptr<Shader>& get_shader(const std::string& name) const;
    const ShaderMap& get_shader_map() const { return shaders; }

    bool has_shader(const std::string& name) const;

private:
    ShaderMap shaders;
};

} // namespace pine
