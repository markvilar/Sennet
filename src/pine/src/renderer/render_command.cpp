#include "render_command.hpp"

#include "pine/pch.hpp"

namespace pine
{

std::unique_ptr<RendererAPI> RenderCommand::s_renderer_api
    = RendererAPI::create();

}
