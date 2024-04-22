#pragma once
#include "Genix/Renderer/RendererContext.h"

/* include Vulkan header BEFORE GLFW */
#include <vulkan/vulkan.h>

struct GLFWwindow;

class VulkanContext : public RendererContext
{
public:
	VulkanContext(GLFWwindow* windowHandle);

	void Init() override;
	void SwapBuffers() override;
	void Shutdown() override;
	
private:
	GLFWwindow* m_WindowHandle;

	// stores information about the Vulkan settings
	VkInstance mInstance{};

	// a drawable “surface” in Vulkan
	VkSurfaceKHR mSurface{};
};
