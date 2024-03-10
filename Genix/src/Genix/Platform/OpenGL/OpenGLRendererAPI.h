#pragma once
#include "Genix/Renderer/RendererAPI.h"

class GENIX_API OpenGLRendererAPI : public RendererAPI
{
public:
	void SetClearColor(const glm::vec4& color) override;
	void Clear() override;

	void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};
