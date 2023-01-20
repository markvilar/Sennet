#include "pine/renderer/shader.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/shader.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

std::unique_ptr<Shader> Shader::create(const std::filesystem::path& filepath)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
		    supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(filepath);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

std::unique_ptr<Shader> Shader::create(const std::string& name,
    const std::string& vertex_source,
    const std::string& fragment_source)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
			supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLShader>(name,
            vertex_source,
            fragment_source);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

void ShaderLibrary::add_shader(const std::string& name,
    const std::shared_ptr<Shader>& shader)
{
    PINE_CORE_ASSERT(!has_shader(name), "Shader {0} already exists.", name);
    m_shaders[name] = shader;
}

void ShaderLibrary::add_shader(const std::shared_ptr<Shader>& shader)
{
    auto& name = shader->get_name();
    add_shader(name, shader);
}

bool ShaderLibrary::load_shader(const std::string& name,
    const std::string filepath)
{
    const std::shared_ptr<Shader> shader = Shader::create(filepath);
    PINE_CORE_ASSERT(shader, "Shader is null.");
    add_shader(name, shader);
    return shader ? true : false;
}

bool ShaderLibrary::load_shader(const std::string& filepath)
{
    const std::shared_ptr<Shader> shader = Shader::create(filepath);
    PINE_CORE_ASSERT(shader, "Shader is null.");
    add_shader(shader);
    return shader ? true : false;
}

const std::shared_ptr<Shader>& ShaderLibrary::get_shader(
    const std::string& name) const
{
    PINE_CORE_ASSERT(has_shader(name), "Shader does not exist.");
    return m_shaders.at(name);
}

bool ShaderLibrary::has_shader(const std::string& name) const
{
    return m_shaders.find(name) != m_shaders.end();
}

} // namespace pine
