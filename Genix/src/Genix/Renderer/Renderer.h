#pragma once
#include "Genix/Renderer/RendererAPI.h"
#include "Genix/Common/PrimitiveTypes.h"

class UniformBuffer;
class Entity;
class Shader;
class VertexArray;

class Renderer
{
public:
	static void Init();
	static void Shutdown();
	
	static void OnWindowResize(uint32 width, uint32 height);
	
	static void BeginScene(const Ref<Entity>& camera);
	static void EndScene();

	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, glm::mat4& transform = glm::mat4(1.0f));

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	static glm::mat4 GetViewProjectionMatrix() { return s_SceneData->CameraBuffer.ViewProjectionMatrix; } 
	static glm::vec3 GetCameraPosition() { return s_SceneData->CameraBuffer.Position; } 

private:
	struct SceneData
	{
		struct CameraData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 Position;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Scope<SceneData> s_SceneData;
};
