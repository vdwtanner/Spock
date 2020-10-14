#pragma once

#include <memory>

#include "VulkanFunctions.h"
#include "LogicalDevice.h"

namespace Spock::vkCore
{

	class SwapChain
	{
	public:
		SwapChain(const VkSwapchainKHR handle, const std::shared_ptr<LogicalDevice> logicalDevice, const VkFormat format, const VkExtent2D extent);
		const VkFormat GetFormat() const;
		const VkExtent2D GetExtent() const;
		const int GetNumImageViews() const;
		const VkImageView& GetImageView(int x) const;
		const VkSwapchainKHR GetSwapChainHandle() const;
		~SwapChain();
	private:
		const VkSwapchainKHR vkSwapChainHandle;
		const std::shared_ptr<LogicalDevice> logicalDevice;
		const VkFormat format;
		const VkExtent2D extent;
		const std::vector<VkImage> images;
		const std::vector<VkImageView> imageViews;

		std::vector<VkImage> FetchImages() const;
		std::vector<VkImageView> CreateImageViews() const;
	};
}