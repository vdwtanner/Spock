#pragma once

#include <vector>
#include <string>

#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore
{
	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(VkPhysicalDevice deviceHandle);
		const VkPhysicalDevice GetVkPhysicalDeviceHandle() const;
		const std::vector<VkExtensionProperties> FetchExtensionProperties() const;
		const VkPhysicalDeviceFeatures FetchPhysicalDeviceFeatures() const;
		const VkPhysicalDeviceProperties FetchPhysicalDeviceProperties() const;
		const std::vector<VkQueueFamilyProperties> FetchQueueFamilyProperties() const;
	private:
		const VkPhysicalDevice deviceHandle;
	};
}


