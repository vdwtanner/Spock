#if defined _WIN32
#include <windows.h>
#endif

#include <vector>

#include "Loader.h"
#include "Logger/Logger.h"
#include "StringUtils.h"
#include "SpockException.h"
#include "VulkanFunctions.h"

namespace Spock::vkCore
{
	using namespace Common;

	Loader::Loader() {
		vulkan_library = nullptr;
		availableExtensions = std::vector< VkExtensionProperties>();
	}

	void Loader::LoadVulkanLibrary() {
		#if defined _WIN32
			vulkan_library = LoadLibrary(Common::StringUtils::StringToWideString("vulkan-1.dll").c_str());
		#elif defined __linux
			vulkan_libary = dlopen("libvulkan.so.1", RTLD_NOW);
		#endif
		if (vulkan_library == nullptr) {
			THROW_EXCEPTION(SpockException, "Could not connect with a Vulkan Runtime Library");
		}
		loadStateBitmask |= LIBRARY_LOADED;
	}

	void Loader::LoadExportedFunctions() {
		ASSERT_USAGE((loadStateBitmask & LIBRARY_LOADED) > 0, "Must call LoadVulkanLibrary() before LoadExportedFunctions().");
		LOG_INFO("\nLoading EXPORTED_VULKAN_FUNCTIONs...");
		#define EXPORTED_VULKAN_FUNCTION(name)\
		name = (PFN_##name)LoadFunction(vulkan_library, #name);\
		if (name == nullptr) {\
			THROW_EXCEPTION(SpockException, "Could not load Vulkan function named:  " #name);\
		} else {\
			LOG_INFO("\tLoaded: " #name);\
		}
		
		//Including this will cause the above macro to be called for all of the EXPORTED_VULKAN_FUNCTIONs in the .inl
		#include "ListOfVulkanFunctions.inl"
		loadStateBitmask |= EXPORTED_FUNCTIONS_LOADED;
	}

	void Loader::LoadGlobalFunctions() {
		ASSERT_USAGE((loadStateBitmask & EXPORTED_FUNCTIONS_LOADED) > 0, "Must call LoadExportedFunctions() before LoadGlobalFunctions().");
		LOG_INFO("\nLoading GLOBAL_VULKAN_FUNCTIONs...");
		#define GLOBAL_LEVEL_VULKAN_FUNCTION(name)\
		name = (PFN_##name)vkGetInstanceProcAddr(nullptr, #name);\
		if (name == nullptr) {\
			THROW_EXCEPTION(SpockException, "Could not load Vulkan function named:  " #name);\
		} else {\
			LOG_INFO("\tLoaded: " #name);\
		}

		#include "ListOfVulkanFunctions.inl"
		loadStateBitmask |= GLOBAL_FUNCTIONS_LOADED;
	}

	void Loader::CheckAvailableExtensions() {
		ASSERT_USAGE((loadStateBitmask & GLOBAL_FUNCTIONS_LOADED) > 0, "Must call LoadGlobalFunctions() before CheckAvailableExtensions().");
		LOG_INFO("\nChecking Available Extensions...");
		uint32_t numExtensions = 0;
		VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to check the number of Vulkan Extensions. Result: " + result);
		}
		availableExtensions.resize(numExtensions);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, &availableExtensions[0]);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to retrieve the list of extension properties. Result: " + result);
		}
		LOG_INFO("Found Extensions:");
		for (auto& ep : availableExtensions) {
			LOG_INFO("\t" + std::string(ep.extensionName) + "  v" + std::to_string(ep.specVersion));
		}
		loadStateBitmask |= CHECKED_AVAILABLE_EXTENSIONS;
	}
}