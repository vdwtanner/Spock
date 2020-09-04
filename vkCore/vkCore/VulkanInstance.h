#pragma once

#include "vkCore/VulkanFunctions.h"

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