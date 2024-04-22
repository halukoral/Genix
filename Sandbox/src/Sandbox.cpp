#include <Genix.h>
#include <Genix/Core/EntryPoint.h>

class Sandbox : public Application
{
public:
	Sandbox(const ApplicationSpecification& specification) : Application(specification)
	{
	}

	~Sandbox() override = default;
};

Application* CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Sandbox";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}