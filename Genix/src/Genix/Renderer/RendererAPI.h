#pragma once

#include "Genix/Renderer/VertexArray.h"
#include <glm/glm.hpp>

class Entity;

class RendererAPI
{
public:
	virtual ~RendererAPI() = default;

	enum class API
	{
		None = 0,
		OpenGL = 1,
		DirectX = 2,
		Vulkan = 3
	};

	struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};
	
	virtual void Init() = 0;
	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void SetActiveTexture(const uint8 slot) = 0;
	virtual void Clear() = 0;
	virtual void ResetFrameBuffer() = 0;
	virtual void SetBlendFunc(BlendFuncArgument arg1, BlendFuncArgument arg2) = 0;
	
	virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

	virtual void RenderCube() = 0;
	virtual void RenderSphere() = 0;
	virtual void RenderQuad() = 0;
	
	static API GetAPI() { return s_API; }
	static Scope<RendererAPI> Create();
	static RenderAPICapabilities& GetCapabilities()
	{
		static RenderAPICapabilities capabilities;
		return capabilities;
	}
	
private:
	static API s_API;
};
