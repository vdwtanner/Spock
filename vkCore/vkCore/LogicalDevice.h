#pragma once

#include <memory>

#include "Common/Functional/Optional.h"
#include "PhysicalDevice.h"
#include "VulkanFunctions.h"

namespace Spock::vkCore
{
	struct QueueFamilyIndices
	{
	public:
		Common::Optional<uint32_t> graphicsFamily;
		Common::Optional<uint32_t> presentFamily;

		QueueFamilyIndices();
		QueueFamilyIndices(const QueueFamilyIndices& other) = default;
		QueueFamilyIndices(QueueFamilyIndices&& other) = default;
		QueueFamilyIndices& operator=(QueueFamilyIndices& other) = default;

		const bool IsComplete() const;
	};

	class LogicalDevice
	{
	public:
		LogicalDevice(const VkDevice vkDeviceHandle, const PhysicalDevice physicalDevice, const std::vector<const char *> extensions, const QueueFamilyIndices queueFamilyIndices);
		~LogicalDevice();
		const void InitPostFunctionLoad();
		const VkDevice GetVkDeviceHandle() const;
		const PhysicalDevice GetPhysicalDevice() const;
		const QueueFamilyIndices GetQueueFamilyIndices() const;
		const VkQueue GetGraphicsQueue() const;
		const VkQueue GetPresentQueue() const;
		const bool IsExtensionSupported(const char* extensionName) const;
	private:
		const VkDevice vkDeviceHandle;
		const PhysicalDevice physicalDevice;
		const QueueFamilyIndices queueFamilyIndices;
		const std::vector<const char*> availableExtensions;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
	};
}