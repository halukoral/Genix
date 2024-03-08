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
		if (Input::IsKeyPressed(GX_KEY_TAB))
			LOG_TRACE("Tab key is pressed (poll)!");
	}
	
	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Event& event) override
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == GX_KEY_TAB)
			{
				LOG_TRACE("Tab key is pressed (event)!");
			}
			LOG_TRACE("{0}", (char)e.GetKeyCode());
		}
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