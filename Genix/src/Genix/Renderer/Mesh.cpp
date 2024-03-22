#include "gxpch.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLMesh.h"

Ref<Mesh> Mesh::Create(std::vector<VertexData> vertices, std::vector<uint32> indices, std::vector<Ref<Texture>> textures)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;
		
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLMesh>(vertices, indices, textures);
		
	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		break;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
