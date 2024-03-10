#pragma once
#include "RenderCommand.h"

class GENIX_API Renderer
{
public:
	static void BeginScene();
	static void EndScene();

	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};
