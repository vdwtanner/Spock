#pragma once

#include <memory>
#include <optional>

#include "PhysicalDevice.h"
#include "VulkanFunctions.h"

namespace Spock::vkCore
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
	};

	class LogicalDevice
	{
	public:
		LogicalDevice(const VkDevice vkDeviceHandle, const std::vector<const char *> extensions, const QueueFamilyIndices queueFamilyIndices);
		~LogicalDevice();
		const void InitPostFunctionLoad();
		const VkDevice GetVkDeviceHandle() const;
		const bool IsExtensionSupported(const char* extensionName) const;
	private:
		const VkDevice vkDeviceHandle;
		const QueueFamilyIndices queueFamilyIndices;
		const std::vector<const char*> availableExtensions;
		VkQueue graphicsQueue;
	};
}