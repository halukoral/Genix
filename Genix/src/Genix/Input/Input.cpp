#include "gxpch.h"
#include "Genix/Core/Application.h"
#include "Genix/Input/Input.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(KeyCode  keycode)
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	const auto state = glfwGetKey(window,(int)keycode);
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode  button)
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	const auto state = glfwGetMouseButton(window, (int)button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
	auto[x, y] = GetMousePosition();
	return x;
}

float Input::GetMouseY()
{
	auto[x, y] = GetMousePosition();
	return y;
}
