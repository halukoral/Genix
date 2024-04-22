#include "gxpch.h"
#include "Genix/Renderer/Shader.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Platform/OpenGL/OpenGLShader.h"

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI())
	{

	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI())
	{

	case RendererAPI::API::None:
		ASSERT_CORE(false, "RendererAPI::None is currently not supported!")
		return nullptr;

	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);

	case RendererAPI::API::DirectX:
		ASSERT_CORE(false, "RendererAPI::DirectX is currently not supported!")
		return nullptr;
	}

	ASSERT_CORE(false, "Unknown RendererAPI!")
	return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
	ASSERT_CORE(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
	auto& name = shader->GetName();
	Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	auto shader = Shader::Create(vertexSrc, fragmentSrc);
	Add(shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
	Add(name, shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	ASSERT_CORE(Exists(name), "Shader not found!");
	return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}