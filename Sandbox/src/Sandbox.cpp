#include <Genix.h>
#include <Genix/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "ModelLayer.h"

class Sandbox : public Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());

		PushLayer(new ModelLayer());
	}

	~Sandbox() override = default;
};

Application* CreateApplication()
{
	return new Sandbox();
}