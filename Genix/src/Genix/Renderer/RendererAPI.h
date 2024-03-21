#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

class RendererAPI
{
public:
	virtual ~RendererAPI() = default;

	enum class API
	{
		None = 0,
		OpenGL = 1,
		DirectX = 2
	};

	virtual void Init() = 0;
	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void SetActiveTexture(const uint8 slot) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

	static API GetAPI() { return s_API; }
	
	static Scope<RendererAPI> Create();
	
private:
	static API s_API;
};
