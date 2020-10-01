#include "vkCore/VulkanInstanceFactory.h"

#include "vkCore/Consts.h"
#include "Common/Exceptions/SpockException.h"
#include "vkCore/VulkanFunctions.h"

#include "vkCore/Include/vulkan_core.h"
#include "Common/Logger/Logger.h"
#include "vkCore/DebugMessenger.h"

namespace Spock::vkCore
{
	using namespace Common;
	VulkanInstanceFactoryImpl::VulkanInstanceFactoryImpl(std::shared_ptr<Loader> loader) : loader(loader) {
	}


	std::unique_ptr<VulkanInstance> VulkanInstanceFactoryImpl::CreateVulkanInstance(const std::string& appName, const Version& appVersion) {

		if (!loader->AreAllExtensionsAvailable(requiredExtensions)) {
			THROW_EXCEPTION(SpockException, "Not all required extensions are available. Check the log for more details.");
		}
		if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport()) {
			THROW_EXCEPTION(SpockException, "Not all validation layers are supported! Check the log for details.");
		}
		VkApplicationInfo appInfo = MakeAppInfo(appName, appVersion);
		auto layers = ENABLE_VALIDATION_LAYERS ? validationLayers : std::vector<const char*>();
		auto extensions = GetExtensions();
		VkInstanceCreateInfo createInfo = MakeVkInstanceCreateInfo(&appInfo, layers, extensions);
		VkInstance vkInstance = MakeVkInstance(&createInfo);
		loader->LoadInstanceLevelFunctions(vkInstance);
		loader->LoadInstanceLevelFunctionsFromExtensions(vkInstance);
		auto vkDebugMessengerHandle = MakeVkDebugUtilsMessenger(vkInstance);
		auto instance = std::make_unique<VulkanInstance>(vkInstance, vkDebugMessengerHandle);
		
		return instance;
	}

	bool VulkanInstanceFactoryImpl::CheckValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				LOG_ERROR("Validation Layer \"" + std::string(layerName) + "\" is not supported!");
				return false;
			}
		}

		return true;
	}

	VkApplicationInfo VulkanInstanceFactoryImpl::MakeAppInfo(const std::string& appName, const Version& appVersion) {
		return {
			VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,				//pnext
			appName.c_str(),
			ToVkVersion(appVersion),
			kEngineName,
			ToVkVersion(kEngineVersion),
			VK_MAKE_VERSION(1,0,0)	//API version
		};
	}

	uint32_t VulkanInstanceFactoryImpl::ToVkVersion(const Version& version) {
		return VK_MAKE_VERSION(version.major, version.minor, version.patch);
	}

	std::vector<const char*> VulkanInstanceFactoryImpl::GetExtensions() {
		auto extensions = std::vector<const char*>(requiredExtensions);
		if (ENABLE_VALIDATION_LAYERS) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	VkInstanceCreateInfo VulkanInstanceFactoryImpl::MakeVkInstanceCreateInfo(
		const VkApplicationInfo* appInfo, 
		const std::vector<const char*>& layers,
		const std::vector<const char*>& availableExtensions
	) {
		return {
			VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,					//flags
			appInfo,
			(uint32_t)layers.size(),
			layers.empty() ? nullptr : &layers[0],
			(uint32_t)availableExtensions.size(),
			availableExtensions.empty() ? nullptr : &availableExtensions[0]
		};
	}

	VkInstance VulkanInstanceFactoryImpl::MakeVkInstance(const VkInstanceCreateInfo* createInfo) {
		VkInstance instance;

		VkResult result = vkCreateInstance(createInfo, nullptr, &instance);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to create Vulkan Instance. Result: " + result);
		}
		return instance;
	}
	VkDebugUtilsMessengerEXT VulkanInstanceFactoryImpl::MakeVkDebugUtilsMessenger(const VkInstance instance) {
		if (!ENABLE_VALIDATION_LAYERS) {
			return VK_NULL_HANDLE;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = Debug::vulkanDebugCallback;
		createInfo.pUserData = nullptr;

		VkDebugUtilsMessengerEXT debugMessengerHandle;
		VK_CALL(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessengerHandle), "Failed to create Debug Messenger");
		
		return debugMessengerHandle;
	}
}