#pragma once

#include "Sennet/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace Sennet
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const uint32_t& width, const uint32_t& height);
	OpenGLTexture2D(const std::string& path);
	~OpenGLTexture2D();

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }

	virtual uint32_t GetRendererID() const override { return m_RendererID; }

	virtual void SetData(void* data, const uint32_t& size) override;

	virtual void Bind(uint32_t slot = 0) const override;
	virtual void Unbind() const override;

private:
	std::string m_Source;
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_RendererID;
	GLenum m_InternalFormat, m_DataFormat;
};

}
