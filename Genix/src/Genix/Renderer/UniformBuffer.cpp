#include "gxpch.h"
#include "Genix/Renderer/UniformBuffer.h"
#include "Genix/Renderer/Renderer.h"

#include "Genix/Platform/OpenGL/OpenGLUniformBuffer.h"

Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLUniformBuffer>(size, binding);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
