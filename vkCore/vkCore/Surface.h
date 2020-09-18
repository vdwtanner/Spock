#pragma once

#include <vector>

#include "VulkanFunctions.h"
#include "PhysicalDevice.h"

namespace Spock::vkCore
{
	class Surface
	{
	public:
		Surface(const VkSurfaceKHR surfaceHandle, const VkInstance vkInstanceHandle);
		~Surface();

		const VkSurfaceCapabilitiesKHR FetchCapabilities(const PhysicalDevice& device) const ;
		const std::vector<VkSurfaceFormatKHR> FetchFormats(const PhysicalDevice& device) const;
		const std::vector<VkPresentModeKHR> FetchPresentModes(const PhysicalDevice& device) const;
		const VkSurfaceKHR GetVkSurfaceHandle() const;
	private:
		const VkSurfaceKHR vkSurfaceHandle;
		const VkInstance vkInstanceHandle;
	};
}


