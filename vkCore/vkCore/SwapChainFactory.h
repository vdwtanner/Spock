#pragma once

#include <memory>
#include "VulkanFunctions.h"
#include "SwapChain.h"

#include "LogicalDevice.h"
#include "Surface.h"

namespace Spock::vkCore
{

	class SwapChainFactory
	{
	public:
		virtual std::unique_ptr<SwapChain> CreateSwapChain(const std::shared_ptr<LogicalDevice> logicalDevice, const Surface* surface, const VkExtent2D desiredExtent, const VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR) = 0;
	};

	class SwapChainFactoryImpl : public SwapChainFactory
	{
	public:
		SwapChainFactoryImpl();
		//Creates a swap chain with the desired presentation mode if available, else FIFO presentation
		std::unique_ptr<SwapChain> CreateSwapChain(const std::shared_ptr<LogicalDevice> logicalDevice, const Surface* surface, const VkExtent2D desiredExtent, const VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR) override;
	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(const PhysicalDevice& physicalDevice, const Surface* surface) const;
		VkPresentModeKHR ChoosePresentMode(const PhysicalDevice& physicalDevice, const Surface* surface, const VkPresentModeKHR desiredPresentMode) const;
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, const VkExtent2D desiredExtent) const;
		uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR capabilities) const;
		VkSwapchainKHR CreateVkSwapchain(const LogicalDevice& logicalDevice, const Surface* surface, const VkSurfaceFormatKHR format, const VkPresentModeKHR presentMode, const VkExtent2D extent, const VkSurfaceCapabilitiesKHR capabilities);
	};
}
