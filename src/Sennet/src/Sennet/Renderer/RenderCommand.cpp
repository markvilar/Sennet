#include "Sennet/Renderer/RenderCommand.hpp"
#include "Sennet/Pch.hpp"

namespace Sennet
{

Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
