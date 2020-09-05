#include "../VulkanInstance.h"

#include "vkCore/VulkanFunctions.h"
#include "Common/SpockException.h"
#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
	using namespace Common;

	VulkanInstance::VulkanInstance(VkInstance handle) : vkInstanceHandle(handle) {
	}

	VulkanInstance::~VulkanInstance() {
		vkDestroyInstance(vkInstanceHandle, nullptr);
	}

	const VkInstance VulkanInstance::GetVkInstanceHandle() const {
		return vkInstanceHandle;
	}

	const std::vector<VulkanPhysicalDevice> VulkanInstance::EnumeratePhysicalDevices() const {
		auto vkPhysicalDevices = EnumerateVkPhysicalDevices();

		auto vulkanPhysicalDevices = std::vector<VulkanPhysicalDevice>();
		for (auto& vkPhysicalDevice : vkPhysicalDevices) {
			vulkanPhysicalDevices.push_back(VulkanPhysicalDevice(vkPhysicalDevice));
		}
		
		return vulkanPhysicalDevices;
	}

	const std::vector<VkPhysicalDevice> VulkanInstance::EnumerateVkPhysicalDevices() const {
		uint32_t numDevices;
		auto result = vkEnumeratePhysicalDevices(vkInstanceHandle, &numDevices, nullptr);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed fetching the number of physical devices with result: " + result);
		} else if (numDevices == 0) {
			THROW_EXCEPTION(SpockException, "No GPU found!");
		}
		LOG_INFO("Found " + std::to_string(numDevices) + " physical devices.");

		auto vkPhysicalDevices = std::vector<VkPhysicalDevice>(numDevices);
		result = vkEnumeratePhysicalDevices(vkInstanceHandle, &numDevices, &vkPhysicalDevices[0]);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed enumerating physical devices with result: " + result);
		}

		return vkPhysicalDevices;
	}
}