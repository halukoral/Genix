#include "gxpch.h"
#include "Genix/Renderer/Texture.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLTexture.h"

Ref<Texture> Texture::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLTexture>(path);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
