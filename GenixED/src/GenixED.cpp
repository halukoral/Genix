#include <Genix.h>
#include <Genix/Core/EntryPoint.h>

#include "EditorLayer.h"

class GenixED : public Application
{
public:
	GenixED() : Application("GenixED")
	{
		PushLayer(new EditorLayer());
	}

	~GenixED() override = default;
};

Application* CreateApplication()
{
	return new GenixED();
}
