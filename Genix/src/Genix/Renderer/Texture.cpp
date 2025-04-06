#include "gxpch.h"
#include "Genix/Renderer/Texture.h"
#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/RendererAPI.h"

#include <stb_image.h>
#include <glad/glad.h>

uint32 Texture::CalculateMipMapCount(uint32 width, uint32 height)
{
	return (uint32)std::floor(std::log2(glm::min(width, height))) + 1;
}

uint32 Texture::GetBPP(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RED32F:  return 4;
	case TextureFormat::RGB:
	case TextureFormat::SRGB:    return 3;
	case TextureFormat::RGBA:    return 4;
	case TextureFormat::RGBA16F: return 2 * 4;
	case TextureFormat::RGBA32F: return 4 * 4;
	}
	ASSERT_CORE(false);
	return 0;
}

static GLenum GenixToOpenGLTextureFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RG:					return GL_RG;
	case TextureFormat::RGB:                return GL_RGB;
	case TextureFormat::RGB16F:             return GL_RGB16F;
	case TextureFormat::RGBA:               return GL_RGBA;
	case TextureFormat::RGBA16F:            return GL_RGBA16F;
	case TextureFormat::RGBA32F:            return GL_RGBA32F;
	case TextureFormat::RG16F:              return GL_RG16F;
	case TextureFormat::RG32F:              return GL_RG32F;
	case TextureFormat::SRGB:               return GL_SRGB;
	case TextureFormat::DEPTH32F:	        return GL_DEPTH32F_STENCIL8;
	case TextureFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
	case TextureFormat::DEPTH24COMP:		return GL_DEPTH_COMPONENT24;
	}
	ASSERT_CORE(false, "Unknown texture format!");
	return 0;
}

Ref<Texture2D> Texture2D::Create(TextureProperties properties)
{
	return CreateRef<Texture2D>(properties);
}

Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb)
{
	return CreateRef<Texture2D>(path, srgb);
}

Ref<TextureCube> TextureCube::Create(TextureProperties properties)
{
	return CreateRef<TextureCube>(properties);
}

Ref<TextureCube> TextureCube::Create(const std::string& path)
{
	return CreateRef<TextureCube>(path);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Texture2D::Texture2D(const TextureProperties& properties)
{
	m_Properties = properties;
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Properties.Wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Properties.Wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTextureParameterf(m_Id, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GenixToOpenGLTextureFormat(m_Properties.InternalFormat),
		(GLsizei)m_Properties.Width,
		(GLsizei)m_Properties.Height,
		0,
		GenixToOpenGLTextureFormat(m_Properties.Format),
		GL_UNSIGNED_BYTE,
		nullptr
	);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(const std::string& path, bool srgb)
{
	m_Path = path;
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	if (stbi_is_hdr(path.c_str()))
	{
		LOG_CORE_INFO("Loading HDR texture {0}, srgb={1}", path, srgb);
		m_ImageData.Data = (uint8*)stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		m_IsHDR = true;
		m_Properties.InternalFormat = TextureFormat::RGBA16F;
	}
	else
	{
		LOG_CORE_INFO("Loading texture {0}, srgb={1}", path, srgb);
		m_ImageData.Data = stbi_load(path.c_str(), &width, &height, &channels, srgb ? STBI_rgb : STBI_rgb_alpha);
		ASSERT_CORE(m_ImageData.Data, "Could not read image!");
		m_Properties.InternalFormat = TextureFormat::RGBA;
	}

	m_Loaded = true;

	m_Properties.Format = m_Properties.InternalFormat;
	m_Properties.Width = width;
	m_Properties.Height = height;

	if (srgb)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		const auto levels = CalculateMipMapCount(m_Properties.Width, m_Properties.Height);
		glTextureStorage2D(m_Id, levels, GL_SRGB8, (GLsizei)m_Properties.Width, (GLsizei)m_Properties.Height);
		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(
			m_Id,
			0,
			0,
			0,
			(GLsizei)m_Properties.Width,
			(GLsizei)m_Properties.Height,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			m_ImageData.Data
		);
		glGenerateTextureMipmap(m_Id);
	}
	else
	{
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		const GLenum internalFormat = GenixToOpenGLTextureFormat(m_Properties.InternalFormat);
		const GLenum format = srgb ? GL_SRGB8 : (m_IsHDR ? GL_RGB : GenixToOpenGLTextureFormat(m_Properties.Format)); // HDR = GL_RGB for now
		const GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			internalFormat,
			(GLsizei)m_Properties.Width,
			(GLsizei)m_Properties.Height,
			0,
			format,
			type,
			m_ImageData.Data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	stbi_image_free(m_ImageData.Data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_Id);
}

void Texture2D::Bind(uint32 slot) const
{
	glBindTextureUnit(slot, m_Id);
}

void Texture2D::Lock()
{
	m_Locked = true;
}

void Texture2D::Unlock()
{
	m_Locked = false;
	glTextureSubImage2D(
		m_Id,
		0,
		0,
		0,
		(GLsizei)m_Properties.Width,
		(GLsizei)m_Properties.Height,
		GenixToOpenGLTextureFormat(m_Properties.Format),
		GL_UNSIGNED_BYTE,
		m_ImageData.Data
	);
}

void Texture2D::Resize(uint32 width, uint32 height)
{
	ASSERT_CORE(m_Locked, "Texture must be locked!");

	m_ImageData.Allocate(width * height * GetBPP(m_Properties.Format));
}

Buffer Texture2D::GetWriteableBuffer()
{
	ASSERT_CORE(m_Locked, "Texture must be locked!");
	return m_ImageData;
}

uint32 Texture2D::GetMipLevelCount() const
{
	return CalculateMipMapCount(m_Properties.Width, m_Properties.Height);
}

//////////////////////////////////////////////////////////////////////////////////
// TextureCube
//////////////////////////////////////////////////////////////////////////////////

TextureCube::TextureCube(const TextureProperties& properties)
{
	m_Properties = properties;
	const uint32 levels = CalculateMipMapCount(m_Properties.Width, m_Properties.Height);

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_Id);

	glTextureStorage2D(
		m_Id,
		levels,
		GenixToOpenGLTextureFormat(m_Properties.InternalFormat),
		(GLsizei)m_Properties.Width,
		(GLsizei)m_Properties.Height
	);

	glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// glTextureParameterf(m_Id, GL_TEXTURE_MAX_ANISOTROPY, 16);
}

TextureCube::TextureCube(const std::string& path)
{
	m_Path = path;
	int width, height, channels;
	stbi_set_flip_vertically_on_load(false);
	m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

	const uint32 faceWidth = m_Properties.Width / 4;
	const uint32 faceHeight = m_Properties.Height / 3;
	ASSERT_CORE(faceWidth == faceHeight, "Non-square faces!");

	std::array<unsigned char*, 6> faces;
	for (auto& face : faces)
		face = new unsigned char[faceWidth * faceHeight * 3]; // 3 BPP

	int faceIndex = 0;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t y = 0; y < faceHeight; y++)
		{
			const size_t yOffset = y + faceHeight;
			for (size_t x = 0; x < faceWidth; x++)
			{
				const size_t xOffset = x + i * faceWidth;
				faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 0];
				faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 1];
				faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 2];
			}
		}
		faceIndex++;
	}

	for (size_t i = 0; i < 3; i++)
	{
		// Skip the middle one
		if (i == 1) continue;

		for (size_t y = 0; y < faceHeight; y++)
		{
			const size_t yOffset = y + i * faceHeight;
			for (size_t x = 0; x < faceWidth; x++)
			{
				const size_t xOffset = x + faceWidth;
				faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 0];
				faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 1];
				faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Properties.Width) * 3 + 2];
			}
		}
		faceIndex++;
	}

	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameterf(m_Id, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

	const auto format = GenixToOpenGLTextureFormat(m_Properties.InternalFormat);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, (GLsizei)faceWidth, (GLsizei)faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto& face : faces)
	{
		delete[] face;
	}

	stbi_image_free(m_ImageData);
}

TextureCube::~TextureCube()
{
	glDeleteTextures(1, &m_Id);
}

void TextureCube::Bind(uint32 slot) const
{
	glBindTextureUnit(slot, m_Id);
}

void TextureCube::GenerateMipMap()
{
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

uint32 TextureCube::GetMipLevelCount() const
{
	return CalculateMipMapCount(m_Properties.Width, m_Properties.Height);
}
