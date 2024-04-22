#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class Buffer;

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

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	virtual void Resize(uint32 width, uint32 height) = 0;
	virtual bool Loaded() const = 0;
	virtual Buffer GetWriteableBuffer() = 0;

	static TextureType GetTextureType() { return TextureType::Texture2D; }
};

class TextureCube : public Texture
{
public:
	static Ref<TextureCube> Create(TextureProperties properties = TextureProperties());
	static Ref<TextureCube> Create(const std::string& path);

	virtual void GenerateMipMap() = 0;
	static TextureType GetTextureType() { return TextureType::TextureCube; }
};