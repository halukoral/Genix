#include "gxpch.h"

#include "Application.h"
#include "Layer.h"
#include "Log.h"

#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

#include <glad/glad.h>



#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    
	case ShaderDataType::Float2:   
	case ShaderDataType::Float3:   
	case ShaderDataType::Float4:   
	case ShaderDataType::Mat3:     
	case ShaderDataType::Mat4:
		return GL_FLOAT;
		
	case ShaderDataType::Int:      
	case ShaderDataType::Int2:     
	case ShaderDataType::Int3:     
	case ShaderDataType::Int4:
		return GL_INT;

	case ShaderDataType::Bool:
		return GL_BOOL;
	}

	ASSERT_CORE(false, "Unknown ShaderDataType!")
	return 0;
}

Application::Application()
{
	ASSERT_CORE(!s_Instance, "Application already exists!");
	s_Instance = this;
	
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	float vertices[3 * 7] = {
		// Vertices			// Color
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	const BufferLayout layout =
	{
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	m_VertexBuffer->SetLayout(layout);
	
	uint32_t index = 0;
	const auto& vertexBufferLayout = m_VertexBuffer->GetLayout();
	const auto& stride = vertexBufferLayout.GetStride();
	for (const auto& element : vertexBufferLayout)
	{
		glEnableVertexAttribArray(index);

		glVertexAttribPointer(
			index,
			(GLint)element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			(GLsizei)stride,
			(const void*)element.Offset);

		index++;
	}
	
	uint32_t indices[3] = { 0, 1, 2 };
	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	vertexSrc = "Assets/Shaders/shader.vert";
	fragmentSrc = "Assets/Shaders/shader.frag";

	m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
}

void Application::Run()
{
	while (m_Running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		
		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();
		
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(e);
		if (e.GetHandled() == true)
		{
			 break;
		}
	}
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
