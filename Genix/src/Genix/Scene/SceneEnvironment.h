#pragma once

#include "Genix/Renderer/Texture.h"

class Environment
{
public:
	Ref<TextureCube> RadianceMap;
	Ref<TextureCube> IrradianceMap;

	Environment() = default;
	Environment(const Ref<TextureCube>& radianceMap, const Ref<TextureCube>& irradianceMap)
		: RadianceMap(radianceMap), IrradianceMap(irradianceMap) {}
};