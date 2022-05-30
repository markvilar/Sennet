#include "Pine/Renderer/Shader.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLShader.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
{

std::unique_ptr<Shader> Shader::Create(const std::filesystem::path& filepath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
		    supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(filepath);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

std::unique_ptr<Shader> Shader::Create(const std::string& name,
    const std::string& vertexSrc, const std::string& fragmentSrc)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
			supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

void ShaderLibrary::Add(const std::string& name,
    const std::shared_ptr<Shader>& shader)
{
    PINE_CORE_ASSERT(!Exists(name), "Shader {0} already exists.", name);
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
{
    auto& name = shader->GetName();
    Add(name, shader);
}

bool ShaderLibrary::Load(const std::string& name, const std::string filepath)
{
    const std::shared_ptr<Shader> shader = Shader::Create(filepath);
    PINE_CORE_ASSERT(shader, "Shader is null.");
    Add(name, shader);
    return shader ? true : false;
}

bool ShaderLibrary::Load(const std::string& filepath)
{
    const std::shared_ptr<Shader> shader = Shader::Create(filepath);
    PINE_CORE_ASSERT(shader, "Shader is null.");
    Add(shader);
    return shader ? true : false;
}

const std::shared_ptr<Shader>& ShaderLibrary::Get(const std::string& name) const
{
    PINE_CORE_ASSERT(Exists(name), "Shader does not exist.");
    return m_Shaders.at(name);
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return m_Shaders.find(name) != m_Shaders.end();
}

} // namespace Pine
