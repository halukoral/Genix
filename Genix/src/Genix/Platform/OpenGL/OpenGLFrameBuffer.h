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
	uint32 GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
	uint32 m_ID = 0;
	uint32 m_ColorAttachment = 0;
	uint32 m_DepthAttachment = 0;
	FramebufferSpecification m_Specification;
};
