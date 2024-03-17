#pragma once
#include "RenderCommand.h"

class GENIX_API Renderer
{
public:

	static void OnWindowResize(uint32 width, uint32 height);
	
	static void BeginScene();
	static void EndScene();

	static void SubmitVAO(const std::shared_ptr<VertexArray>& vertexArray);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};
