#include "../VulkanPhysicalDevice.h"

#include "vkCore/VulkanFunctions.h"
#include "Common/SpockException.h"
#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
	using namespace Common;

	VulkanPhysicalDevice::VulkanPhysicalDevice(const VkPhysicalDevice vkDeviceHandle)
		: vkDeviceHandle(vkDeviceHandle) {
		LoadExtensions();
	}

	const VkPhysicalDevice VulkanPhysicalDevice::GetVkPhysicalDeviceHandle() const {
		return vkDeviceHandle;
	}

	const VkPhysicalDeviceFeatures VulkanPhysicalDevice::FetchPhysicalDeviceFeatures() const {
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(vkDeviceHandle, &features);
		return features;
	}

	const VkPhysicalDeviceProperties VulkanPhysicalDevice::FetchPhysicalDeviceProperties() const {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(vkDeviceHandle, &properties);
		return properties;
	}

	const std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::FetchQueueFamilyProperties() const {
		uint32_t numQueueFamilyProperties;
		vkGetPhysicalDeviceQueueFamilyProperties(vkDeviceHandle, &numQueueFamilyProperties, nullptr);

		auto queueFamilyProperties = std::vector<VkQueueFamilyProperties>(numQueueFamilyProperties);
		vkGetPhysicalDeviceQueueFamilyProperties(vkDeviceHandle, &numQueueFamilyProperties, &queueFamilyProperties[0]);
		return queueFamilyProperties;
	}

	const std::vector<VkExtensionProperties> VulkanPhysicalDevice::GetExtensionProperties() const{
		return availableExtensions;
	}

	const bool VulkanPhysicalDevice::AreAllExtensionsSupported(const std::vector<const char*> desiredExtensions) const{
		for (auto& extension : desiredExtensions) {
			if (!IsExtensionSupported(extension)) {
				LOG_INFO(std::string(extension) + " is not supported.");
				return false;
			}
		}
		return true;
	}

	void VulkanPhysicalDevice::LoadExtensions() {
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

	const bool VulkanPhysicalDevice::IsExtensionSupported(const char* extension) const {
		for (auto& availableExtension : availableExtensions) {
			if (strcmp(extension, availableExtension.extensionName) == 0) {
				return true;
			}
		}
		return false;
	}
}
