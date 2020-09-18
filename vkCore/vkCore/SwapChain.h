#pragma once

#include <memory>

#include "VulkanFunctions.h"
#include "LogicalDevice.h"

namespace Spock::vkCore
{

	class SwapChain
	{
	public:
		SwapChain(const VkSwapchainKHR handle, const std::shared_ptr<LogicalDevice> logicalDevice);
		~SwapChain();
	private:
		const VkSwapchainKHR vkSwapChain;
		const std::shared_ptr<LogicalDevice> logicalDevice;
	};
}