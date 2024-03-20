#include <Genix.h>
#include <Genix/EntryPoint.h>

#include "ExampleLayer.h"

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
	return new Sandbox();
}