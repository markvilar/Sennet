#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Pine/Utils/Math.hpp"

namespace Pine
{

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, const int value) const = 0;
    virtual void SetIntArray(const std::string& name, const int* values,
        const uint32_t count) const = 0;
    virtual void SetFloat(const std::string& name, const float value) const = 0;
    virtual void SetFloat3(
        const std::string& name, const Vec3& value) const = 0;
    virtual void SetFloat4(
        const std::string& name, const Vec4& value) const = 0;
    virtual void SetMat4(const std::string& name, const Mat4& value) const = 0;

    virtual const std::string& GetName() const = 0;

    static std::unique_ptr<Shader> Create(const std::string& filepath);
    static std::unique_ptr<Shader> Create(const std::string& name,
        const std::string& vertexSrc, const std::string& fragmentSrc);
};

class ShaderLibrary
{
public:
    ShaderLibrary() = default;
    ~ShaderLibrary() = default;

    void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
    void Add(const std::shared_ptr<Shader>& shader);

    bool Load(const std::string& name, const std::string filepath);
    bool Load(const std::string& filepath);

    const std::shared_ptr<Shader>& Get(const std::string& name) const;

    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};

} // namespace Pine
