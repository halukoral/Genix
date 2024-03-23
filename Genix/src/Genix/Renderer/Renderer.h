#pragma once
#include "Genix/Renderer/RendererAPI.h"
#include "Genix/Common/PrimitiveTypes.h"

class Camera;
class Model;
class Shader;
class VertexArray;

class Renderer
{
public:
	static void Init();
	static void Shutdown();
	
	static void OnWindowResize(uint32 width, uint32 height);
	
	static void BeginScene(const Camera& camera);
	static void EndScene();

	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, glm::mat4& transform = glm::mat4(1.0f));
	static void Submit(const Ref<Shader>& shader, const Ref<Model>& Model, glm::mat4& transform = glm::mat4(1.0f));

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	static Scope<SceneData> s_SceneData;
};
