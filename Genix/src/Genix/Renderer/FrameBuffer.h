#pragma once
#include "Genix/Common/PrimitiveTypes.h"

struct FramebufferSpecification
{
	uint32 Width, Height;
	uint32 Samples = 1;

	bool SwapChainTarget = false;
};

class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual uint32 GetColorAttachmentRendererID() const = 0;

	virtual const FramebufferSpecification& GetSpecification() const = 0;

	static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};
