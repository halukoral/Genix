#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class UniformBuffer
{
public:
	virtual ~UniformBuffer() {}
	virtual void SetData(const void* data, uint32 size, uint32 offset = 0) = 0;

	static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
};
