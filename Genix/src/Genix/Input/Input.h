#pragma once

#include "Genix/Input/KeyCodes.h"
#include "Genix/Input/MouseCodes.h"

class Input
{
public:
	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(MouseCode button);
	
	static std::pair<float, float> GetMousePosition();
	
	static float GetMouseX();
	static float GetMouseY();
};
