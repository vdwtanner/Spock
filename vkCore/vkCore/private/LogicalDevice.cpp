#include "../LogicalDevice.h"

namespace Spock::vkCore
{
	using namespace Common;

	QueueFamilyIndices::QueueFamilyIndices() :
		graphicsFamily(Optional<uint32_t>::empty()),
		presentFamily(Optional<uint32_t>::empty()){

	}

	const bool QueueFamilyIndices::IsComplete() const {
		return graphicsFamily.IsPresent() && presentFamily.IsPresent();
	}

	LogicalDevice::LogicalDevice(const VkDevice vkDeviceHandle, const PhysicalDevice physicalDevice, const std::vector<const char*> extensions, const QueueFamilyIndices queueFamilyIndices)
		: vkDeviceHandle(vkDeviceHandle), physicalDevice(physicalDevice), availableExtensions(extensions), queueFamilyIndices(queueFamilyIndices) {
		graphicsQueue = VK_NULL_HANDLE;
		presentQueue = VK_NULL_HANDLE;
	}

	LogicalDevice::~LogicalDevice() {
		vkDestroyDevice(vkDeviceHandle, nullptr);
	}

	const void LogicalDevice::InitPostFunctionLoad() {
		queueFamilyIndices.graphicsFamily.Apply([&](uint32_t q) { vkGetDeviceQueue(vkDeviceHandle, q, 0, &graphicsQueue); });
		queueFamilyIndices.presentFamily.Apply([&](uint32_t q) { vkGetDeviceQueue(vkDeviceHandle, q, 0, &presentQueue); });
	}

	const VkDevice LogicalDevice::GetVkDeviceHandle() const {
		return vkDeviceHandle;
	}

	const PhysicalDevice LogicalDevice::GetPhysicalDevice() const {
		return physicalDevice;
	}

	const QueueFamilyIndices LogicalDevice::GetQueueFamilyIndices() const {
		return queueFamilyIndices;
	}

	const VkQueue LogicalDevice::GetGraphicsQueue() const {
		return graphicsQueue;
	}

	const VkQueue LogicalDevice::GetPresentQueue() const {
		return presentQueue;
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