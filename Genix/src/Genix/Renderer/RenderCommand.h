#pragma once
#include "RendererAPI.h"

class GENIX_API RenderCommand
{
public:
	static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	static void SetClearColor(const glm::vec4& color);
	static void Clear();

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

private:
	static Scope<RendererAPI> s_RendererAPI;
};
