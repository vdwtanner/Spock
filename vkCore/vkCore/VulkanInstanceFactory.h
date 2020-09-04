#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "vkCore/VulkanInstance.h"
#include "vkCore/Loader.h"
#include "Common/Version.h"

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
			VK_KHR_SURFACE_EXTENSION_NAME
		};

		uint32_t ToVkVersion(const Common::Version& version);
		VkApplicationInfo MakeAppInfo(const std::string& appName, const Common::Version& appVersion);
		VkInstanceCreateInfo MakeVkInstanceCreateInfo(const VkApplicationInfo* appInfo, const std::vector<const char*>& layers, const std::vector<const char*>& extensions);
		VkInstance MakeVkInstance(const VkInstanceCreateInfo* createInfo);
	};
}

