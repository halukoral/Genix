#pragma once
#include "Genix/Common/PrimitiveTypes.h"
#include "Genix/Core/Buffer.h"

enum class TextureFormat
{
	None = 0,
	RED32F,
	RG,
	RGB,
	RGB16F,
	RGBA,
	RGBA16F,
	RGBA32F,
	RG16F,
	RG32F,

	SRGB,

	DEPTH32F,
	DEPTH24STENCIL8,
	DEPTH24COMP,

	// Defaults
	Depth = DEPTH24STENCIL8,
};

enum class TextureUsage
{
	None = 0,
	Texture,
	Attachment,
	Storage
};


enum class TextureFilter
{
	None = 0,
	Linear,
	Nearest
};

enum class TextureWrap
{
	None = 0,
	Clamp = 1,
	Repeat = 2
};

enum class TextureType
{
	None = 0,
	Texture2D,
	TextureCube
};

struct TextureProperties
{
	TextureFormat Format = TextureFormat::RGBA;
	TextureFormat InternalFormat = TextureFormat::RGBA;

	TextureUsage Usage = TextureUsage::None;
	TextureFilter Filter = TextureFilter::Linear;
	TextureWrap Wrap = TextureWrap::Repeat;
	
	bool GenerateMips = true;
	bool SRGB = false;

	uint32 Width = 512;
	uint32 Height = 512;
};

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void Bind(uint32 slot = 0) const = 0;

	virtual uint32 GetId() const = 0;

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	virtual uint32 GetMipLevelCount() const = 0;
	
	std::string GetPath() const						{ return m_Path; }
	void SetPath(const std::string& path)			{ m_Path = path; }

	TextureFormat GetTextureFormat() const			{ return m_Properties.Format; }
	TextureFormat GetInternalTextureFormat() const	{ return m_Properties.InternalFormat; }
	TextureWrap GetTextureWrap() const				{ return m_Properties.Wrap; }
	
	static uint32 CalculateMipMapCount(uint32 width, uint32 height);
	static uint32 GetBPP(TextureFormat format);

protected:
	std::string m_Path;
	TextureProperties m_Properties;
};

class Texture2D : public Texture
{
public:
	static Ref<Texture2D> Create(TextureProperties properties = TextureProperties());
	static Ref<Texture2D> Create(const std::string& path, bool srgb = false);

	static TextureType GetTextureType() { return TextureType::Texture2D; }

	Texture2D(const TextureProperties& properties);
	Texture2D(const std::string& path, bool srgb);
	~Texture2D() override;

	void Bind(uint32 slot = 0) const override;
	
	uint32 GetWidth() const override { return m_Properties.Width; }
	uint32 GetHeight() const override { return m_Properties.Height; }
	// This function currently returns the expected number of mips based on image size, not present mips in data
	uint32 GetMipLevelCount() const override;

	void Lock();
	void Unlock();

	void Resize(uint32 width, uint32 height);
	Buffer GetWriteableBuffer();

	bool Loaded() const { return m_Loaded; }

	uint32 GetId() const override { return m_Id; }

private:
	uint32 m_Id;
	Buffer m_ImageData;
	
	bool m_IsHDR = false;
	bool m_Locked = false;
	bool m_Loaded = false;
	
};

class TextureCube : public Texture
{
public:
	static Ref<TextureCube> Create(TextureProperties properties = TextureProperties());
	static Ref<TextureCube> Create(const std::string& path);

	TextureCube(const TextureProperties& properties);
	TextureCube(const std::string& path);
	~TextureCube() override;

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