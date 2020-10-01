#pragma once

#include "vkCore/VulkanInstance.h"
#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore::Debug
{
	VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
}
