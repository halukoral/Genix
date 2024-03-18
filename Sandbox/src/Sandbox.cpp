#include <Genix.h>

#include "Genix/Renderer/Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			// Vertices			// Color
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	
		const BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
	
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
	
		uint32 indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_Shader.reset(new Shader("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag"));
	}

	void OnUpdate(TimeStep ts) override
	{
		m_Camera.OnUpdate(ts);
		
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}
		
		Renderer::EndScene();
	}
	
	void OnImGuiRender() override
	{
	}

	void OnEvent(Event& e) override
	{
		m_Camera.OnEvent(e);
	}

private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	Camera m_Camera;
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() override = default;
};

Application* CreateApplication()
{
	return new Sandbox;
}