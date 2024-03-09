#pragma once

class GENIX_API RendererContext
{
public:
	virtual ~RendererContext() = default;
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
};