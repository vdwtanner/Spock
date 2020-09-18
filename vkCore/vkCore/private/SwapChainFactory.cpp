#include "../SwapChainFactory.h"

#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
	SwapChainFactoryImpl::SwapChainFactoryImpl() {
	}

	std::unique_ptr<SwapChain> SwapChainFactoryImpl::CreateSwapChain(
		const std::shared_ptr<LogicalDevice> logicalDevice,
		const Surface* surface,
		const VkExtent2D desiredExtent,
		const VkPresentModeKHR desiredPresentMode
	) {
		auto physicalDevice = logicalDevice->GetPhysicalDevice();
		auto capabilities = surface->FetchCapabilities(physicalDevice);

		auto surfaceFormat = ChooseSurfaceFormat(physicalDevice, surface);
		auto presentMode = ChoosePresentMode(physicalDevice, surface, desiredPresentMode);
		auto swapExtent = ChooseSwapExtent(capabilities, desiredExtent);

		auto vkSwapchain = CreateVkSwapchain(*logicalDevice, surface, surfaceFormat, presentMode, swapExtent, capabilities);
		auto swapChain = std::make_unique<SwapChain>(vkSwapchain, logicalDevice, surfaceFormat.format, swapExtent);
		return swapChain;
	}

	VkSurfaceFormatKHR SwapChainFactoryImpl::ChooseSurfaceFormat(const PhysicalDevice& logicalDevice, const Surface* surface) const {
		auto availableFormats = surface->FetchFormats(logicalDevice);
		for (const auto& availableFormat : availableFormats) {
			//Prefer 32bit color in SRGB color space.
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		//Otherwise just return the first format available
		return availableFormats[0];
	}

	VkPresentModeKHR SwapChainFactoryImpl::ChoosePresentMode(
		const PhysicalDevice& physicalDevice,
		const Surface* surface,
		const VkPresentModeKHR desiredPresentMode
	) const {
		auto availableModes = surface->FetchPresentModes(physicalDevice);
		for (const auto mode : availableModes) {
			if (mode == desiredPresentMode) {
				return mode;
			}
		}
		//This present mode is always guaranteed to exist
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChainFactoryImpl::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, const VkExtent2D desiredExtent) const {
		//if the width is UINT32_MAX, that means that we have to choose our own extent
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}

		VkExtent2D extent;
		extent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, desiredExtent.height));
		extent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, desiredExtent.width));
		return extent;
	}

	uint32_t SwapChainFactoryImpl::ChooseImageCount(const VkSurfaceCapabilitiesKHR capabilities) const {
		//It is recommended to have at least one more than the minimum so that we don't wait on the driver
		auto imageCount = capabilities.minImageCount + 1;
		//0 is a magic number that means that there is no maximum
		if (capabilities.maxImageCount > 0 && capabilities.maxImageCount < imageCount) {
			return capabilities.maxImageCount;
		}
		return imageCount;
	}

	VkSwapchainKHR SwapChainFactoryImpl::CreateVkSwapchain(
		const LogicalDevice& logicalDevice,
		const Surface* surface,
		const VkSurfaceFormatKHR surfaceFormat,
		const VkPresentModeKHR presentMode,
		const VkExtent2D extent,
		const VkSurfaceCapabilitiesKHR capabilities
	) {
		auto indices = logicalDevice.GetQueueFamilyIndices();
		ASSERT_USAGE(indices.IsComplete(), "Graphics and Present queueFamilyIndices are required to create a swapchain.");
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface->GetVkSurfaceHandle();
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.presentMode = presentMode;
		createInfo.minImageCount = ChooseImageCount(capabilities);
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;	//This is where we could support resizing

		uint32_t queueFamilyIndices[] = {indices.graphicsFamily.Get(), indices.presentFamily.Get()};
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			//queueFamilyIndex values not needed for exclusive mode
		}

		VkSwapchainKHR swapchain;
		vkCreateSwapchainKHR(logicalDevice.GetVkDeviceHandle(), &createInfo, nullptr, &swapchain);

		return swapchain;
	}
}