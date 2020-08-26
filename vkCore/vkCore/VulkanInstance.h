#pragma once

#include "Include/vulkan.h"

namespace Spock::vkCore
{
	class VulkanInstance
	{
	public:
		VulkanInstance(VkInstance handle);
		~VulkanInstance();
		const VkInstance GetVkInstanceHandle() const;

	private:
		const VkInstance vkInstanceHandle;
	};
}