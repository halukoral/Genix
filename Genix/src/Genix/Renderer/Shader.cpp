#include "gxpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLShader.h"

Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    ASSERT_CORE(false, "RendererAPI::None is currently not supported!") return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
	case RendererAPI::API::DirectX: ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!") return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}
