#include "gxpch.h"
#include "Material.h"

#include "Renderer.h"
#include "RendererAPI.h"

Ref<Material> Material::Create(const Ref<Shader>& shader, const std::string& name)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		//return CreateRef<OpenGLMaterial>(shader, name);
		return nullptr;

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

Ref<Material> Material::Copy(const Ref<Material>& other, const std::string& name)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case RendererAPI::API::OpenGL:
		//return CreateRef<OpenGLMaterial>(other, name);
		return nullptr;

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
