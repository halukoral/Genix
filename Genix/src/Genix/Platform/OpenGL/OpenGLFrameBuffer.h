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

	uint32 GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
	uint32 m_ID;
	uint32 m_ColorAttachment;
	uint32 m_DepthAttachment;
	FramebufferSpecification m_Specification;
};
