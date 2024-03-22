#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class TextureType
{
	Diffuse,
	Specular,
	Normal,
	Height
};

class Texture
{
public:
	virtual ~Texture() = default;

	virtual uint32 GetId() const = 0;
	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	virtual void Bind(uint32 slot = 0) const = 0;

	std::string GetPath() const { return m_Path; }
	void SetPath(const std::string& path) { m_Path = path; }

	TextureType GetTextureType() const { return m_TextureType; }
	void SetTextureType(const TextureType type) { m_TextureType = type; }
	
	static Ref<Texture> Create(const std::string& path);

protected:
	TextureType m_TextureType;
	std::string m_Path;
};
