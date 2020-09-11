#pragma once

#include <vector>
#include <string>

#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore
{
	class PhysicalDevice
	{
	public:
		PhysicalDevice(const VkPhysicalDevice vkDeviceHandle);
		const VkPhysicalDevice GetVkPhysicalDeviceHandle() const;
		const VkPhysicalDeviceFeatures FetchPhysicalDeviceFeatures() const;
		const VkPhysicalDeviceProperties FetchPhysicalDeviceProperties() const;
		const std::vector<VkQueueFamilyProperties> FetchQueueFamilyProperties() const;
		const std::vector<VkExtensionProperties> GetExtensionProperties() const;
		const bool AreAllExtensionsSupported(const std::vector<const char*> desiredExtensions) const;
	private:
		const VkPhysicalDevice vkDeviceHandle;
		std::vector<VkExtensionProperties> availableExtensions;

		void LoadExtensions();
		const bool IsExtensionSupported(const char* extension) const;
	};
}


