#include "ExampleLayer.h"

ExampleLayer::ExampleLayer(): Layer("Example")
{
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	m_SquareVA = VertexArray::Create();
		
	Ref<VertexBuffer> squareVB;
	squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32 squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32));
	m_SquareVA->SetIndexBuffer(squareIB);
		
	m_Shader = Shader::Create("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");
	m_Texture = Texture::Create("Assets/Textures/bricks2.jpg");
	m_AwesomeFaceTexture = Texture::Create("Assets/Textures/awesomeface.png");
		
	m_Shader->Bind();
	m_Shader->GLSetUniform_Int("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(TimeStep ts)
{
	m_Camera.OnUpdate(ts);
		
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_Camera);

	m_Texture->Bind();
	Renderer::Submit(m_Shader, m_SquareVA);
	m_AwesomeFaceTexture->Bind();
	Renderer::Submit(m_Shader, m_SquareVA);
		
	Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnEvent(Event& e)
{
	m_Camera.OnEvent(e);
}
