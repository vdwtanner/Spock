#pragma once

#include "VulkanFunctions.h"

namespace Spock::vkCore
{
	class Surface
	{
	public:
		Surface(const VkSurfaceKHR surfaceHandle, const VkInstance vkInstanceHandle);
		~Surface();
		const VkSurfaceKHR GetVkSurfaceHandle() const;
	private:
		const VkSurfaceKHR vkSurfaceHandle;
		const VkInstance vkInstanceHandle;
	};
}


