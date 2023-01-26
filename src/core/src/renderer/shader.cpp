#include "pine/renderer/shader.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/shader.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

std::unique_ptr<Shader> create_shader(const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
		    supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return opengl::create_shader(vertex_file, fragment_file);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

std::unique_ptr<Shader> create_shader(const std::string& name,
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
        return opengl::create_shader(name, vertex_source, fragment_source);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

void ShaderLibrary::add_shader(const std::string& name,
    const std::shared_ptr<Shader>& shader)
{
    PINE_CORE_ASSERT(!has_shader(name), "Shader {0} already exists.", name);
    shaders[name] = shader;
}

void ShaderLibrary::add_shader(const std::shared_ptr<Shader>& shader)
{
    auto& name = shader->get_name();
    add_shader(name, shader);
}

bool ShaderLibrary::load_shader(const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file)
{
    const std::shared_ptr<Shader> shader =
        create_shader(vertex_file, fragment_file);
    PINE_CORE_ASSERT(shader, "Shader is null.");
    add_shader(shader->get_name(), shader);
    return shader ? true : false;
}

const std::shared_ptr<Shader>& ShaderLibrary::get_shader(
    const std::string& name) const
{
    PINE_CORE_ASSERT(has_shader(name), "Shader does not exist.");
    return shaders.at(name);
}

bool ShaderLibrary::has_shader(const std::string& name) const
{
    return shaders.find(name) != shaders.end();
}

} // namespace pine
