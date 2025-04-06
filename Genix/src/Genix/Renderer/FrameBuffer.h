#pragma once
#include "Genix/Common/PrimitiveTypes.h"

enum class FramebufferTextureFormat
{
	None,

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
	Framebuffer(const FramebufferSpecification& spec);
	virtual ~Framebuffer();
	void Invalidate();

	void Bind();
	void Unbind();

	void Resize(uint32 width, uint32 height);
	int  ReadPixel(uint32 attachmentIndex, int x, int y);
	void ClearAttachment(uint32 attachmentIndex, int value);
	
	uint32 GetColorAttachmentRendererID(uint32 index = 0) const;

	const FramebufferSpecification& GetSpecification() const { return m_Specification; }

	static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

private:
	uint32 m_ID = 0;
	uint32 m_DepthAttachment = 0;
	std::vector<uint32> m_ColorAttachments;
	
	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	FramebufferSpecification m_Specification;
};
