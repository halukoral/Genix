#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class GENIX_API Texture
{
public:
	virtual ~Texture() = default;

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	virtual void Bind(uint32 slot = 0) const = 0;

	static Ref<Texture> Create(const std::string& path);
};
