#include <Genix.h>

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

	void OnEvent(Event& event) override
	{
		LOG_TRACE("{0}", event);
	}

};
class Sandbox : public Application
{
public:
	Sandbox() { PushLayer(new ExampleLayer()); }
	~Sandbox() {}
};

Application* CreateApplication()
{
	return new Sandbox;
}