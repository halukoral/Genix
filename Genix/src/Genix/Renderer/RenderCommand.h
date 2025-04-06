#pragma once
#include "RendererAPI.h"

class RenderCommand
{
public:
	static void Init() { s_RendererAPI->Init(); }

	static void SetActiveTexture(const uint8 slot);
	static void SetClearColor(const glm::vec4& color);
	static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);

	static void Clear();
	static void DrawIndexed(const Ref<VertexArray>& vertexArray);
	static void ResetFrameBuffer();
	static void SetBlendFunc(BlendFuncArgument arg1, BlendFuncArgument arg2);

	static void RenderCube();
	static void RenderSphere();
	static void RenderQuad();

private:
	static Scope<RendererAPI> s_RendererAPI;
};
