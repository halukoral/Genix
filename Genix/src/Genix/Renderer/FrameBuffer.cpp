﻿#include "gxpch.h"
#include "Genix/Renderer/FrameBuffer.h"
#include "Genix/Renderer/Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLFrameBuffer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLFramebuffer>(spec);
	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}