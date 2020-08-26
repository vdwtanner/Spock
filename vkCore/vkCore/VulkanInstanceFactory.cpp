#include "VulkanInstanceFactory.h"

#include "vkCore/Consts.h"
#include "SpockException.h"
#include "vkCore/VulkanFunctions.h"

#include "vkCore/Include/vulkan_core.h"

namespace Spock::vkCore
{
	using namespace Common;
	VulkanInstanceFactory::VulkanInstanceFactory(std::shared_ptr<Loader> loader) : loader(loader) {
	}


	std::unique_ptr<VulkanInstance> VulkanInstanceFactory::CreateVulkanInstance(const std::string& appName, const Version& appVersion, const std::vector<const char*>& desiredExtenisions) {
		if (!loader->AreAllExtensionsAvailable(desiredExtenisions)) {
			THROW_EXCEPTION(SpockException, "Not all requested extensions are available. Check the log for more details.");
		}
		VkApplicationInfo appInfo = MakeAppInfo(appName, appVersion);
		auto layers = std::vector<const char*>();
		VkInstanceCreateInfo createInfo = MakeVkInstanceCreateInfo(&appInfo, layers, desiredExtenisions);
		VkInstance vkInstance = MakeVkInstance(&createInfo);
		auto instance = std::make_unique<VulkanInstance>(vkInstance);
		loader->LoadInstanceLevelFunctions(instance.get());
		return instance;
	}

	VkApplicationInfo VulkanInstanceFactory::MakeAppInfo(const std::string& appName, const Version& appVersion) {
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

	uint32_t VulkanInstanceFactory::ToVkVersion(const Version& version) {
		return VK_MAKE_VERSION(version.major, version.minor, version.patch);
	}

	VkInstanceCreateInfo VulkanInstanceFactory::MakeVkInstanceCreateInfo(const VkApplicationInfo* appInfo, const std::vector<const char*>& layers, const std::vector<const char*>& extensions) {
		return {
			VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,					//flags
			appInfo,
			(uint32_t)layers.size(),
			layers.empty() ? nullptr : &layers[0],
			(uint32_t)extensions.size(),
			extensions.empty() ? nullptr : &extensions[0]
		};
	}

	VkInstance VulkanInstanceFactory::MakeVkInstance(const VkInstanceCreateInfo* createInfo) {
		VkInstance instance;

		VkResult result = vkCreateInstance(createInfo, nullptr, &instance);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to create Vulcan Instance. Result: " + result);
		}
		return instance;
	}
}