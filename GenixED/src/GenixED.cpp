#include <Genix.h>
#include <Genix/Core/EntryPoint.h>

#include "EditorLayer.h"

class GenixED : public Application
{
public:
	GenixED(const ApplicationSpecification& specification) : Application(specification)
	{
		PushLayer(new EditorLayer());
	}

	~GenixED() override = default;
};

Application* CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "GenixED";
	spec.WorkingDirectory = "../GenixED";
	spec.CommandLineArgs = args;

	return new GenixED(spec);
}
