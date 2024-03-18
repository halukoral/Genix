#include "gxpch.h"
#include "Input_GLFW.h"

#include "Genix/Application.h"
#include "GLFW/glfw3.h"

Scope<Input> Input::s_Instance = CreateScope<Input_GLFW>();

bool Input_GLFW::IsKeyPressedImpl(int keycode)
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	const auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input_GLFW::IsMouseButtonPressedImpl(int button)
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	const auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input_GLFW::GetMousePositionImpl()
{
	const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input_GLFW::GetMouseXImpl()
{
	auto[x, y] = GetMousePositionImpl();
	return x;
}

float Input_GLFW::GetMouseYImpl()
{
	auto[x, y] = GetMousePositionImpl();
	return y;
}
