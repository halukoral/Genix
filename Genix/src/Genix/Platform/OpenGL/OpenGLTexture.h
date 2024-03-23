#pragma once
#include "Genix/Renderer/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const uint32 width, const uint32 height);
	OpenGLTexture(const std::string& path);
	~OpenGLTexture() override;

	uint32 GetWidth() const override { return  m_Width; }
	uint32 GetHeight() const override { return  m_Height; }
	uint32 GetId() const override { return m_ID; }

	void Bind(uint32 slot = 0) const override;

private:
	std::string m_Path;
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_ID;
	uint32 m_InternalFormat = 0;
	uint32 m_DataFormat = 0;
};
