#pragma once

#include <vector>

#include "vkCore/VulkanFunctions.h"
#include "vkCore/VulkanPhysicalDevice.h"

namespace Spock::vkCore
{
	class VulkanInstance
	{
	public:
		VulkanInstance(VkInstance handle);
		~VulkanInstance();
		const VkInstance GetVkInstanceHandle() const;
		const std::vector<VulkanPhysicalDevice> EnumeratePhysicalDevices() const;
	private:
		const VkInstance vkInstanceHandle;

		const std::vector<VkPhysicalDevice> EnumerateVkPhysicalDevices() const;
	};
}