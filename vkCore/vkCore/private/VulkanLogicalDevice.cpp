#include "../VulkanLogicalDevice.h"
#include "..\VulkanLogicalDevice.h"

namespace Spock::vkCore
{
	VulkanLogicalDevice::VulkanLogicalDevice(const VkDevice vkDeviceHandle, const std::vector<const char*> extensions, const QueueFamilyIndices queueFamilyIndices)
		: vkDeviceHandle(vkDeviceHandle), availableExtensions(extensions), queueFamilyIndices(queueFamilyIndices) {
	}

	VulkanLogicalDevice::~VulkanLogicalDevice() {
		vkDestroyDevice(vkDeviceHandle, nullptr);
	}
	const VkDevice VulkanLogicalDevice::GetVkDeviceHandle() const {
		return vkDeviceHandle;
	}

	const bool VulkanLogicalDevice::IsExtensionSupported(const char* extensionName) const {
		for (auto extension : availableExtensions) {
			if (strcmp(extension, extensionName) != 0) {
				return false;
			}
		}
		return true;
	}
}