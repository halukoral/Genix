#include "gxpch.h"
#include "Genix/Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

OpenGLTexture::OpenGLTexture(const uint32 width, const uint32 height)
	: m_Width(width), m_Height(height)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture::OpenGLTexture(const std::string& path) : m_Path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	ASSERT_CORE(data, "Failed to load image!")
	m_Width = width;
	m_Height = height;

	m_InternalFormat = m_DataFormat = 0;
	
	if (channels == 4)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RGB;
	}
	else if (channels == 1)
	{
		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RED;
	}

	ASSERT_CORE(m_InternalFormat & m_DataFormat, "Format not supported!");
	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_ID);
}

void OpenGLTexture::Bind(const uint32 slot) const
{
	glBindTextureUnit(slot, m_ID);
}
