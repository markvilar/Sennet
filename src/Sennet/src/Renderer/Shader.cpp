#include "Sennet/Renderer/Shader.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Renderer.hpp"

#include "Sennet/Platform/OpenGL/OpenGLShader.hpp"

namespace Sennet
{

Ref<Shader> Shader::Create(const std::string& filepath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(filepath);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name,
    const std::string& vertexSrc,
    const std::string& fragmentSrc)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
    SENNET_CORE_ASSERT(!Exists(name), "Shader {0} already exists.", name);
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    auto& name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& name,
    const std::string filepath)
{
    auto shader = Shader::Create(filepath);
    SENNET_CORE_ASSERT(shader, "Shader is null.");
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
{
    SENNET_CORE_ASSERT(Exists(name), "Shader does not exist.");
    return m_Shaders.at(name);
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return m_Shaders.contains(name);
}

} // namespace Sennet
