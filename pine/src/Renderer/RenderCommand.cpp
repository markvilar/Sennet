#include "Pine/Renderer/RenderCommand.hpp"
#include "Pine/Pch.hpp"

namespace Pine
{

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
