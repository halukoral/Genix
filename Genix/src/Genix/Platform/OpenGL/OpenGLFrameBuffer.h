#pragma once
#include "Genix/Renderer/FrameBuffer.h"

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const FramebufferSpecification& spec);
	~OpenGLFramebuffer() override;

	void Invalidate();

	void Bind() override;
	void Unbind() override;

	void Resize(uint32 width, uint32 height) override;
	int ReadPixel(uint32 attachmentIndex, int x, int y) override;
	void ClearAttachment(uint32 attachmentIndex, int value) override;

	uint32 GetColorAttachmentRendererID(uint32 index = 0) const override;

	const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
	uint32 m_ID = 0;
	uint32 m_DepthAttachment = 0;
	std::vector<uint32> m_ColorAttachments;
	
	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	FramebufferSpecification m_Specification;
};
