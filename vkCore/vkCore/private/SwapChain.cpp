#include "../SwapChain.h"

namespace Spock::vkCore
{
	using namespace Common;

	SwapChain::SwapChain(
		const VkSwapchainKHR handle,
		const std::shared_ptr<LogicalDevice> logicalDevice,
		const VkFormat format,
		const VkExtent2D extent
	) : vkSwapChainHandle(handle),
		logicalDevice(logicalDevice), 
		format(format), extent(extent),
		images(FetchImages()),
		imageViews(CreateImageViews()) {
	}

	const VkFormat SwapChain::GetFormat() const {
		return format;
	}

	const VkExtent2D SwapChain::GetExtent() const {
		return extent;
	}

	const int SwapChain::GetNumImageViews() const {
		return static_cast<int>(imageViews.size());
	}

	const VkImageView& SwapChain::GetImageView(int x) const {
		return imageViews[x];
	}

	SwapChain::~SwapChain() {
		for (auto& imageView : imageViews) {
			vkDestroyImageView(logicalDevice->GetVkDeviceHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice->GetVkDeviceHandle(), vkSwapChainHandle, nullptr);
	}

	std::vector<VkImage> SwapChain::FetchImages() const {
		uint32_t numImages;
		auto result = vkGetSwapchainImagesKHR(logicalDevice->GetVkDeviceHandle(), vkSwapChainHandle, &numImages, nullptr);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch number of swap chain images");
		}

		auto swapChainImages = std::vector<VkImage>(numImages);
		result = vkGetSwapchainImagesKHR(logicalDevice->GetVkDeviceHandle(), vkSwapChainHandle, &numImages, &swapChainImages[0]);

		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to fetch swap chain images");
		}
		return swapChainImages;
	}

	std::vector<VkImageView> SwapChain::CreateImageViews() const {
		auto views = std::vector<VkImageView>(images.size());
		for (int i = 0; i < images.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;

			auto result = vkCreateImageView(logicalDevice->GetVkDeviceHandle(), &createInfo, nullptr, &views[i]);
			if (result != VkResult::VK_SUCCESS) {
				THROW_EXCEPTION(SpockException, "Failed to create Image View");
			}
		}
		return views;
	}
}