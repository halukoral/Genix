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
		None,
		OpenGL = 1,
	};

	struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};
	
	void Init();
	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void SetClearColor(const glm::vec4& color);
	void SetActiveTexture(const uint8 slot);
	void Clear();
	void ResetFrameBuffer();
	void SetBlendFunc(BlendFuncArgument arg1, BlendFuncArgument arg2);
	
	void DrawIndexed(const Ref<VertexArray>& vertexArray);

	void RenderCube();
	void RenderSphere();
	void RenderQuad();
	
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
