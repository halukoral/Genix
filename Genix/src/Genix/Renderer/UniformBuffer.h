#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class UniformBuffer
{
public:
	UniformBuffer(uint32 size, uint32 binding);
	virtual ~UniformBuffer();
	
	void SetData(const void* data, uint32 size, uint32 offset = 0);

	static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
private:
	uint32 m_Id = 0;
};
