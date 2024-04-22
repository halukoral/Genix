#include "gxpch.h"
#include "Genix/Renderer/Texture.h"
#include "Genix/Renderer/Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLTexture2D.h"

uint32 Texture::CalculateMipMapCount(uint32 width, uint32 height)
{
	return (uint32)std::floor(std::log2(glm::min(width, height))) + 1;
}

uint32 Texture::GetBPP(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RED32F:  return 4;
	case TextureFormat::RGB:
	case TextureFormat::SRGB:    return 3;
	case TextureFormat::RGBA:    return 4;
	case TextureFormat::RGBA16F: return 2 * 4;
	case TextureFormat::RGBA32F: return 4 * 4;
	}
	ASSERT_CORE(false);
	return 0;
}

Ref<Texture2D> Texture2D::Create(TextureProperties properties)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLTexture2D>(properties);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
		
	case RendererAPI::API::Vulkan:
		ASSERT_CORE(false, "RendererAPI::Vulkan is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLTexture2D>(path, srgb);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
		
	case RendererAPI::API::Vulkan:
		ASSERT_CORE(false, "RendererAPI::Vulkan is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

Ref<TextureCube> TextureCube::Create(TextureProperties properties)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLTextureCube>(properties);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
		
	case RendererAPI::API::Vulkan:
		ASSERT_CORE(false, "RendererAPI::Vulkan is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

Ref<TextureCube> TextureCube::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLTextureCube>(path);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!");
		return nullptr;
		
	case RendererAPI::API::Vulkan:
		ASSERT_CORE(false, "RendererAPI::Vulkan is currently not supported!");
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
