#include "../PhysicalDevice.h"

#include "vkCore/VulkanFunctions.h"
#include "Common/Exceptions/SpockException.h"
#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
	using namespace Common;

	PhysicalDevice::PhysicalDevice(const VkPhysicalDevice vkDeviceHandle)
		: vkDeviceHandle(vkDeviceHandle) {
		LoadExtensions();
	}

	const VkPhysicalDevice PhysicalDevice::GetVkPhysicalDeviceHandle() const {
		return vkDeviceHandle;
	}

	const VkPhysicalDeviceFeatures PhysicalDevice::FetchPhysicalDeviceFeatures() const {
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(vkDeviceHandle, &features);
		return features;
	}

	const VkPhysicalDeviceProperties PhysicalDevice::FetchPhysicalDeviceProperties() const {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(vkDeviceHandle, &properties);
		return properties;
	}

	const std::vector<VkQueueFamilyProperties> PhysicalDevice::FetchQueueFamilyProperties() const {
		uint32_t numQueueFamilyProperties;
		vkGetPhysicalDeviceQueueFamilyProperties(vkDeviceHandle, &numQueueFamilyProperties, nullptr);

		auto queueFamilyProperties = std::vector<VkQueueFamilyProperties>(numQueueFamilyProperties);
		vkGetPhysicalDeviceQueueFamilyProperties(vkDeviceHandle, &numQueueFamilyProperties, &queueFamilyProperties[0]);
		return queueFamilyProperties;
	}

	const std::vector<VkExtensionProperties> PhysicalDevice::GetExtensionProperties() const{
		return availableExtensions;
	}

	const bool PhysicalDevice::AreAllExtensionsSupported(const std::vector<const char*> desiredExtensions) const{
		for (auto& extension : desiredExtensions) {
			if (!IsExtensionSupported(extension)) {
				LOG_INFO(std::string(extension) + " is not supported.");
				return false;
			}
		}
		return true;
	}

	void PhysicalDevice::LoadExtensions() {
		uint32_t numExtensions;
		auto result = vkEnumerateDeviceExtensionProperties(vkDeviceHandle, nullptr, &numExtensions, nullptr);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed fetching the number of extensions for device with result: " + result);
		}

		availableExtensions.resize(numExtensions);
		result = vkEnumerateDeviceExtensionProperties(vkDeviceHandle, nullptr, &numExtensions, &availableExtensions[0]);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed enumerating extensions for device with result: " + result);
		}
	}

	const bool PhysicalDevice::IsExtensionSupported(const char* extension) const {
		for (auto& availableExtension : availableExtensions) {
			if (strcmp(extension, availableExtension.extensionName) == 0) {
				return true;
			}
		}
		return false;
	}
}
