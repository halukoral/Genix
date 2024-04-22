#pragma once
#include "Genix/Renderer/UniformBuffer.h"

class OpenGLUniformBuffer : public UniformBuffer
{
public:
	OpenGLUniformBuffer(uint32 size, uint32 binding);
	virtual ~OpenGLUniformBuffer();

	void SetData(const void* data, uint32 size, uint32 offset = 0) override;

private:
	uint32 m_Id = 0;
};
