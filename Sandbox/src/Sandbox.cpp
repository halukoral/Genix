#include <Genix.h>

#include "Genix/Platform/OpenGL/OpenGLShader.h"
#include "Genix/Renderer/Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		m_SquareVA.reset(VertexArray::Create());
		
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32 squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32)));
		m_SquareVA->SetIndexBuffer(squareIB);
		
		m_Shader.reset(Shader::Create("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag"));
		m_Texture = Texture::Create("Assets/Textures/bricks2.jpg");
		
		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniform_Int("u_Texture", 0);
	}

	void OnUpdate(TimeStep ts) override
	{
		m_Camera.OnUpdate(ts);
		
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		m_Texture->Bind();
		Renderer::Submit(m_Shader, m_SquareVA);
		
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
	Ref<Shader> m_Shader;
	Ref<VertexArray> m_SquareVA;
	Ref<Texture> m_Texture;
	
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