#pragma once
#include "Camera.h"
#include "RenderCommand.h"

class Shader;

class GENIX_API Renderer
{
public:
	static void OnWindowResize(uint32 width, uint32 height);
	
	static void BeginScene(const Camera& camera);
	static void EndScene();

	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	static SceneData* m_SceneData;
};
