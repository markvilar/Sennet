#include "Sennet/Pch.hpp"
#include "Sennet/Renderer/RendererAPI.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Sennet
{

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
	switch (s_API)
	{
		case RendererAPI::API::None:    
			SENNET_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:  
			return CreateScope<OpenGLRendererAPI>();
	}

	SENNET_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}
