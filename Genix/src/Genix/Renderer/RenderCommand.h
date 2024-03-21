#pragma once
#include "RendererAPI.h"

class RenderCommand
{
public:
	static void Init() { s_RendererAPI->Init(); }
	static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	static void SetClearColor(const glm::vec4& color);
	static void Clear();
	static void SetActiveTexture(const uint8 slot);
	
	static void DrawIndexed(const Ref<VertexArray>& vertexArray);

private:
	static Scope<RendererAPI> s_RendererAPI;
};
