#include <Genix.h>
#include "imgui/imgui.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		LOG_INFO("ExampleLayer::Update");
	}
	
	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Event& event) override
	{
		LOG_TRACE("{0}", event);
	}

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