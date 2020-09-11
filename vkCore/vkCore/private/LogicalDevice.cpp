#include "../LogicalDevice.h"

namespace Spock::vkCore
{
	LogicalDevice::LogicalDevice(const VkDevice vkDeviceHandle, const std::vector<const char*> extensions, const QueueFamilyIndices queueFamilyIndices)
		: vkDeviceHandle(vkDeviceHandle), availableExtensions(extensions), queueFamilyIndices(queueFamilyIndices) {
	}

	LogicalDevice::~LogicalDevice() {
		vkDestroyDevice(vkDeviceHandle, nullptr);
	}
	const void LogicalDevice::InitPostFunctionLoad() {
		if (queueFamilyIndices.graphicsFamily.has_value()) {
			vkGetDeviceQueue(vkDeviceHandle, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
		} else {
			graphicsQueue = VK_NULL_HANDLE;
		}
	}

	const VkDevice LogicalDevice::GetVkDeviceHandle() const {
		return vkDeviceHandle;
	}

	const bool LogicalDevice::IsExtensionSupported(const char* extensionName) const {
		for (auto extension : availableExtensions) {
			if (strcmp(extension, extensionName) != 0) {
				return false;
			}
		}
		return true;
	}
}