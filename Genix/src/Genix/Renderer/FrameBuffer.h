#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RGBA16F,
	RED_INTEGER,
	
	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	// TODO: filtering/wrap
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
	uint32 Width, Height;
	uint32 Samples = 1;
	FramebufferAttachmentSpecification Attachments;
	bool SwapChainTarget = false;
};

// A framebuffer is the most “visible” buffer type for a user:
// the final picture shown on the screen is created in a framebuffer
// and the intermediate results of rendering steps are stored in framebuffers too.
class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void Resize(uint32 width, uint32 height) = 0;
	virtual int ReadPixel(uint32 attachmentIndex, int x, int y) = 0;
	virtual void ClearAttachment(uint32 attachmentIndex, int value) = 0;
	
	virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const = 0;

	virtual const FramebufferSpecification& GetSpecification() const = 0;

	static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};
