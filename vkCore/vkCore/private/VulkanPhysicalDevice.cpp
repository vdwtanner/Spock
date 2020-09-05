#include "../VulkanPhysicalDevice.h"

#include "vkCore/VulkanFunctions.h"
#include "Common/SpockException.h"

namespace Spock::vkCore
{
	using namespace Common;

	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice deviceHandle)
		: deviceHandle(deviceHandle) {
	}

	const VkPhysicalDevice VulkanPhysicalDevice::GetVkPhysicalDeviceHandle() const {
		return deviceHandle;
	}

	const std::vector<VkExtensionProperties> VulkanPhysicalDevice::FetchExtensionProperties() const{
		uint32_t numExtensions;
		auto result = vkEnumerateDeviceExtensionProperties(deviceHandle, nullptr, &numExtensions, nullptr);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed fetching the number of extensions for device with result: " + result);
		}

		auto extensions = std::vector<VkExtensionProperties>(numExtensions);
		result = vkEnumerateDeviceExtensionProperties(deviceHandle, nullptr, &numExtensions, &extensions[0]);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed enumerating extensions for device with result: " + result);
		}

		return extensions;
	}

	const VkPhysicalDeviceFeatures VulkanPhysicalDevice::FetchPhysicalDeviceFeatures() const{
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(deviceHandle, &features);
		return features;
	}

	const VkPhysicalDeviceProperties VulkanPhysicalDevice::FetchPhysicalDeviceProperties() const{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(deviceHandle, &properties);
		return properties;
	}

	const std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::FetchQueueFamilyProperties() const{
		uint32_t numQueueFamilyProperties;
		vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle, &numQueueFamilyProperties, nullptr);

		auto queueFamilyProperties = std::vector<VkQueueFamilyProperties>(numQueueFamilyProperties);
		vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle, &numQueueFamilyProperties, &queueFamilyProperties[0]);
		return queueFamilyProperties;
	}
}
