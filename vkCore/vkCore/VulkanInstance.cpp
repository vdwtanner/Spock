#include "VulkanInstance.h"

#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore
{
	VulkanInstance::VulkanInstance(VkInstance handle) : vkInstanceHandle(handle) {
	}

	VulkanInstance::~VulkanInstance() {
		vkDestroyInstance(vkInstanceHandle, nullptr);
	}

	const VkInstance VulkanInstance::GetVkInstanceHandle() const {
		return vkInstanceHandle;
	}
}