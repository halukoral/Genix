#include <Genix.h>

#include "imgui/imgui.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(), m_CameraPosition(0.0f)
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
		m_SquareVA.reset(VertexArray::Create());
	
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32 squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_Shader.reset(new Shader("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag"));
		m_BlueShader.reset(new Shader("Assets/Shaders/blueShader.vert", "Assets/Shaders/blueShader.frag"));
	}

	void OnUpdate(Timestep ts) override
	{
		if (Input::IsKeyPressed(GX_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(GX_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Input::IsKeyPressed(GX_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(GX_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;


		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		
		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_BlueShader, m_SquareVA);
		Renderer::Submit(m_Shader, m_VertexArray);

		Renderer::EndScene();
	}
	
	void OnImGuiRender() override
	{
	}

	void OnEvent(Event& event) override
	{
	}

private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	std::shared_ptr<Shader> m_BlueShader;
	std::shared_ptr<VertexArray> m_SquareVA;

	Camera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() override {}
};

Application* CreateApplication()
{
	return new Sandbox;
}