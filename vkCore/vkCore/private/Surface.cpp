#include "..\Surface.h"

#include "Common/SpockException.h"

namespace Spock::vkCore
{
	using namespace Common;

	Surface::Surface(const VkSurfaceKHR surfaceHandle, const VkInstance instanceHandle) 
		: vkSurfaceHandle(surfaceHandle), vkInstanceHandle(instanceHandle) {
	}

	Surface::~Surface() {
		vkDestroySurfaceKHR(vkInstanceHandle, vkSurfaceHandle, nullptr);
	}

	const VkSurfaceCapabilitiesKHR Surface::FetchCapabilities(const PhysicalDevice& device) const {
		VkSurfaceCapabilitiesKHR capabilities;
		auto result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.GetVkPhysicalDeviceHandle(), GetVkSurfaceHandle(), &capabilities);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch surface capabilities with error code: " + std::to_string(result));
		}

		return capabilities;
	}

	const std::vector<VkSurfaceFormatKHR> Surface::FetchFormats(const PhysicalDevice& device) const {
		uint32_t numFormats;
		auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDeviceHandle(), GetVkSurfaceHandle(), &numFormats, nullptr);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch number of surface formats with error code: " + std::to_string(result));
		}

		auto formats = std::vector<VkSurfaceFormatKHR>(numFormats);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetVkPhysicalDeviceHandle(), GetVkSurfaceHandle(), &numFormats, &formats[0]);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch surface formats with error code: " + std::to_string(result));
		}

		return formats;
	}

	const std::vector<VkPresentModeKHR> Surface::FetchPresentModes(const PhysicalDevice& device) const {
		uint32_t numPresentationModes;
		auto result = vkGetPhysicalDeviceSurfacePresentModesKHR(
			device.GetVkPhysicalDeviceHandle(), GetVkSurfaceHandle(), &numPresentationModes, nullptr);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch number of presentation modes with error code: " + std::to_string(result));
		}

		auto modes = std::vector<VkPresentModeKHR>(numPresentationModes);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(
			device.GetVkPhysicalDeviceHandle(), GetVkSurfaceHandle(), &numPresentationModes, &modes[0]);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch presentation modes with error code: " + std::to_string(result));
		}

		return modes;
	}

	const VkSurfaceKHR Surface::GetVkSurfaceHandle() const {
		return vkSurfaceHandle;
	}
}

