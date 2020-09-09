#pragma once

#include <memory>
#include <optional>

#include "VulkanPhysicalDevice.h"
#include "VulkanFunctions.h"

namespace Spock::vkCore
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
	};

	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice(const VkDevice vkDeviceHandle, const std::vector<const char *> extensions, const QueueFamilyIndices queueFamilyIndices);
		~VulkanLogicalDevice();
		const VkDevice GetVkDeviceHandle() const;
		const bool IsExtensionSupported(const char* extensionName) const;
	private:
		const VkDevice vkDeviceHandle;
		const QueueFamilyIndices queueFamilyIndices;
		const std::vector<const char*> availableExtensions;
	};
}


