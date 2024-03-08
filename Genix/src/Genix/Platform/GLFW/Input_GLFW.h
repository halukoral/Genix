#pragma once
#include "Genix/Input.h"

class GENIX_API Input_GLFW : public Input
{
protected:
	bool IsKeyPressedImpl(int keycode) override;
	bool IsMouseButtonPressedImpl(int button) override;

	std::pair<float, float> GetMousePositionImpl() override;

	float GetMouseXImpl() override;
	float GetMouseYImpl() override;	
};
