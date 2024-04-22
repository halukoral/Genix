#include "gxpch.h"
#include "Genix/Platform/Vulkan/VulkanContext.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanContext::VulkanContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	ASSERT_CORE(windowHandle, "Window handle is null!")
}

void VulkanContext::Init()
{
	VkResult result = VK_ERROR_UNKNOWN;

	VkApplicationInfo mAppInfo{};
	mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	mAppInfo.pNext = nullptr;
	mAppInfo.pApplicationName = "Genix Vulkan App";
	mAppInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
	mAppInfo.pEngineName = "Genix";
	mAppInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	mAppInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 1, 0);

	uint32_t extensionCount = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	if (extensionCount == 0)
	{
		ASSERT_CORE(false, "No Vulkan extensions found, need at least 'VK_KHR_surface'");
		return;
	}

	LOG_INFO("%s: Found %u Vulkan extensions\n", __FUNCTION__, extensionCount);
	for (int i = 0; i < extensionCount; ++i)
	{
		LOG_INFO("%s: %s\n", __FUNCTION__, std::string(extensions[i]).c_str());
	}

	VkInstanceCreateInfo mCreateInfo{};
	mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	mCreateInfo.pNext = nullptr;
	mCreateInfo.pApplicationInfo = &mAppInfo;
	mCreateInfo.enabledExtensionCount = extensionCount;
	mCreateInfo.ppEnabledExtensionNames = extensions;
	mCreateInfo.enabledLayerCount = 0;

	result = vkCreateInstance(&mCreateInfo, nullptr, &mInstance);
	if (result != VK_SUCCESS)
	{
		ASSERT_CORE(false, "Could not create Vulkan instance");
		return;
	}

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0)
	{
		ASSERT_CORE(false, "No Vulkan capable GPU found");
		return;
	}

	std::vector<VkPhysicalDevice> devices;
	devices.resize(physicalDeviceCount);
	vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, devices.data());

	LOG_CORE_INFO("%s: Found %u physical device(s)\n", __FUNCTION__, physicalDeviceCount);

	if (glfwCreateWindowSurface(mInstance, m_WindowHandle, nullptr, &mSurface) != VK_SUCCESS)
	{
		ASSERT_CORE(0, "Could not create Vulkan surface");
	}
}

void VulkanContext::SwapBuffers()
{
}

void VulkanContext::Shutdown()
{
	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroyInstance(mInstance, nullptr);
}
