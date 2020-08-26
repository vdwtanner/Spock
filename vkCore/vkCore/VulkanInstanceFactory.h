#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "vkCore/VulkanInstance.h"
#include "vkCore/Loader.h"
#include "Version.h"

namespace Spock::vkCore
{
	class VulkanInstanceFactory
	{
	public:
		VulkanInstanceFactory(std::shared_ptr<Loader> loader);
		std::unique_ptr<VulkanInstance> CreateVulkanInstance(const std::string& appName, const Common::Version& appVersion, const std::vector<const char*>& desiredExtenisions);
	private:
		std::shared_ptr<Loader> loader;
		uint32_t ToVkVersion(const Common::Version& version);
		VkApplicationInfo MakeAppInfo(const std::string& appName, const Common::Version& appVersion);
		VkInstanceCreateInfo MakeVkInstanceCreateInfo(const VkApplicationInfo* appInfo, const std::vector<const char*>& layers, const std::vector<const char*>& extensions);
		VkInstance MakeVkInstance(const VkInstanceCreateInfo* createInfo);
	};
}

