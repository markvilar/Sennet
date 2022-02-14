#include "Pine/Renderer/RenderCommand.hpp"

#include "Pine/Pch.hpp"

namespace Pine
{

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI =
    RendererAPI::Create();

}
