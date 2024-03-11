#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

class GENIX_API RendererAPI
{
public:
	virtual ~RendererAPI() = default;

	enum class API
	{
		None = 0,
		OpenGL = 1,
		DirectX = 2
	};

	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	static API GetAPI() { return s_API; }
	
private:
	static API s_API;
};
