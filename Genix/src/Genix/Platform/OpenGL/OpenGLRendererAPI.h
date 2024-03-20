#pragma once
#include "Genix/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	void Init() override;
	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
	void SetClearColor(const glm::vec4& color) override;
	void Clear() override;

	void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};
