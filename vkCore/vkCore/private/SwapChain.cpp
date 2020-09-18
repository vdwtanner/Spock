#include "../SwapChain.h"

namespace Spock::vkCore
{
	SwapChain::SwapChain(const VkSwapchainKHR handle, const std::shared_ptr<LogicalDevice> logicalDevice) 
		: vkSwapChain(handle), logicalDevice(logicalDevice) {
	}

	SwapChain::~SwapChain() {
		vkDestroySwapchainKHR(logicalDevice->GetVkDeviceHandle(), vkSwapChain, nullptr);
	}
}