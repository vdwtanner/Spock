#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "vkCore/VulkanInstance.h"
#include "vkCore/Loader.h"
#include "Common/Version.h"

#include "PortedExtensionNames.inl"

namespace Spock::vkCore
{
	class VulkanInstanceFactory
	{
	public:
		virtual std::unique_ptr<VulkanInstance> CreateVulkanInstance(const std::string& appName, const Common::Version& appVersion) = 0;
	};

	class VulkanInstanceFactoryImpl : public VulkanInstanceFactory
	{
	public:
		VulkanInstanceFactoryImpl(std::shared_ptr<Loader> loader);
		std::unique_ptr<VulkanInstance> CreateVulkanInstance(const std::string& appName, const Common::Version& appVersion) override;
	private:
		std::shared_ptr<Loader> loader;
		const std::vector<const char*> requiredExtensions = {
			VK_KHR_SURFACE_EXTENSION_NAME,
			SPOCK_KHR_PORTED_SURFACE_EXTENSION_NAME
		};
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

	#ifdef _DEBUG
		const bool ENABLE_VALIDATION_LAYERS = true;
	#else
		const bool ENABLE_VALIDATION_LAYERS = false;
	#endif

		bool CheckValidationLayerSupport();
		VkApplicationInfo MakeAppInfo(const std::string& appName, const Common::Version& appVersion);
		uint32_t ToVkVersion(const Common::Version& version);
		std::vector<const char*> GetExtensions();
		VkInstanceCreateInfo MakeVkInstanceCreateInfo(const VkApplicationInfo* appInfo, const std::vector<const char*>& layers, const std::vector<const char*>& availableExtensions);
		VkInstance MakeVkInstance(const VkInstanceCreateInfo* createInfo);
		VkDebugUtilsMessengerEXT MakeVkDebugUtilsMessenger(const VkInstance instance);
	};
}
