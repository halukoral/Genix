#pragma once
#include "Genix/Core/Buffer.h"
#include "Genix/Renderer/Texture.h"

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const TextureProperties& properties);
	OpenGLTexture2D(const std::string& path, bool srgb);
	~OpenGLTexture2D() override;

	void Bind(uint32 slot = 0) const override;
	
	uint32 GetWidth() const override { return m_Properties.Width; }
	uint32 GetHeight() const override { return m_Properties.Height; }
	// This function currently returns the expected number of mips based on image size, not present mips in data
	uint32 GetMipLevelCount() const override;

	void Lock() override;
	void Unlock() override;

	void Resize(uint32 width, uint32 height) override;
	Buffer GetWriteableBuffer() override;

	bool Loaded() const override { return m_Loaded; }

	uint32 GetId() const override { return m_Id; }

private:
	uint32 m_Id;

	Buffer m_ImageData;
	
	bool m_IsHDR = false;
	bool m_Locked = false;
	bool m_Loaded = false;
};

class OpenGLTextureCube : public TextureCube
{
public:
	OpenGLTextureCube(const TextureProperties& properties);
	OpenGLTextureCube(const std::string& path);
	~OpenGLTextureCube() override;

	void Bind(uint32 slot = 0) const override;
	void GenerateMipMap();

	uint32 GetWidth() const override { return m_Properties.Width; }
	uint32 GetHeight() const override { return m_Properties.Height; }
	// This function currently returns the expected number of mips based on image size,
	// not present mips in data
	uint32 GetMipLevelCount() const override;

	uint32 GetId() const override { return m_Id; }
	
private:
	uint32 m_Id;

	unsigned char* m_ImageData;
};
