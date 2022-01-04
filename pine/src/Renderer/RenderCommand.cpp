#include "Pine/Pch.hpp"
#include "Pine/Renderer/RenderCommand.hpp"

namespace Pine
{

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
