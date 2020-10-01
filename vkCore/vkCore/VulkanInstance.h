#pragma once

#include <vector>

#include "vkCore/VulkanFunctions.h"
#include "vkCore/PhysicalDevice.h"

namespace Spock::vkCore
{
	class VulkanInstance
	{
	public:
		VulkanInstance(VkInstance handle, VkDebugUtilsMessengerEXT vkDebugMessengerHandle);
		~VulkanInstance();
		const VkInstance GetVkInstanceHandle() const;
		const std::vector<PhysicalDevice> EnumeratePhysicalDevices() const;
	private:
		const VkInstance vkInstanceHandle;
		const VkDebugUtilsMessengerEXT vkDebugMessengerHandle;

		const std::vector<VkPhysicalDevice> EnumerateVkPhysicalDevices() const;
	};
}