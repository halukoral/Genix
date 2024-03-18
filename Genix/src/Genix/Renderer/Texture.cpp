#include "gxpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLTexture.h"

Ref<Texture> Texture::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture>(path);
	case RendererAPI::API::DirectX: ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!"); return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
