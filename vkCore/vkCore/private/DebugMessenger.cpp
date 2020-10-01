#include "..\DebugMessenger.h"

#include "Common/Logger/Logger.h"

namespace Spock::vkCore::Debug
{
	VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData
	) {
		switch (messageSeverity) {
			//Diagnostic messages
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: LOG_INFO(pCallbackData->pMessage); break;
			//Note that we don't request INFO messages in VulkanInstanceFactory since INFO is more for creation of resources
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: LOG_WARN(pCallbackData->pMessage); break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: LOG_ERROR(pCallbackData->pMessage); break;
		}

		//The callback returns a boolean that indicates if the Vulkan call that triggered the validation layer message should be aborted.
		//This should be false as it is mainly used for testing validation layers
		return VK_FALSE;
	}

}
