#if defined _WIN32
#include <windows.h>
#elif defined __linux
#include <dlfcn.h>
#endif

#include <vector>
#include <string>

#include "vkCore/Loader.h"
#include "Common/Logger/Logger.h"
#include "Common/StringUtils.h"
#include "Common/SpockException.h"
#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore
{
	using namespace Common;

	LoaderImpl::LoaderImpl() {
		vulkan_library = nullptr;
		availableExtensions = std::vector< VkExtensionProperties>();
	}

	LoaderImpl::~LoaderImpl() {
		#if defined _WIN32
			FreeLibrary(vulkan_library);
		#elif defined __linux
			dlclose(vulkan_library);
		#endif
		vulkan_library = nullptr;
	}

	void LoaderImpl::LoadVulkanLibrary() {
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

	void LoaderImpl::LoadExportedFunctions() {
		ASSERT_USAGE((loadStateBitmask & LIBRARY_LOADED) > 0, "Must call LoadVulkanLibrary() before LoadExportedFunctions().");
		LOG_INFO("\nLoading EXPORTED_VULKAN_FUNCTIONs...");
		#define EXPORTED_VULKAN_FUNCTION(name)												\
		name = (PFN_##name)LoadFunction(vulkan_library, #name);								\
		if (name == nullptr) {																\
			THROW_EXCEPTION(SpockException, "Could not load Vulkan function named: " #name);\
		} else {																			\
			LOG_INFO("\tLoaded: " #name);													\
		}
		
		//Including this will cause the above macro to be called for all of the EXPORTED_VULKAN_FUNCTIONs in the .inl
		#include "vkCore/ListOfVulkanFunctions.inl"
		loadStateBitmask |= EXPORTED_FUNCTIONS_LOADED;
	}

	void LoaderImpl::LoadGlobalFunctions() {
		ASSERT_USAGE((loadStateBitmask & EXPORTED_FUNCTIONS_LOADED) > 0, "Must call LoadExportedFunctions() before LoadGlobalFunctions().");
		LOG_INFO("\nLoading GLOBAL_VULKAN_FUNCTIONs...");
		#define GLOBAL_LEVEL_VULKAN_FUNCTION(name)											\
		name = (PFN_##name)vkGetInstanceProcAddr(nullptr, #name);							\
		if (name == nullptr) {																\
			THROW_EXCEPTION(SpockException, "Could not load global-level Vulkan function named: " #name);\
		} else {																			\
			LOG_INFO("\tLoaded: " #name);													\
		}

		#include "vkCore/ListOfVulkanFunctions.inl"
		loadStateBitmask |= GLOBAL_FUNCTIONS_LOADED;
	}

	void LoaderImpl::DiscoverAvailableExtensions() {
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
		loadStateBitmask |= DISCOVERED_AVAILABLE_EXTENSIONS;
	}

	void LoaderImpl::LoadInstanceLevelFunctions(const VulkanInstance* instance) {
		ASSERT_USAGE((loadStateBitmask & GLOBAL_FUNCTIONS_LOADED) > 0, "Must call GLOBAL_FUNCTIONS_LOADED() before LoadInstanceLevelFunctions().");
		LOG_INFO("\nLoading INSTANCE_LEVEL_VULKAN_FUNCTIONS...");
		auto handle = instance->GetVkInstanceHandle();
		#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)										\
		name = (PFN_##name)vkGetInstanceProcAddr(handle, #name);							\
		if (name == nullptr) {																\
			THROW_EXCEPTION(SpockException, "Could not load instance-level Vulkan function named: " #name);\
		} else {																			\
			LOG_INFO("\tLoaded: " #name);													\
		}
		
		#include "vkCore/ListOfVulkanFunctions.inl"
	}

	void LoaderImpl::LoadInstanceLevelFunctionsFromExtensions(const VulkanInstance* instance) {
		ASSERT_USAGE((loadStateBitmask & GLOBAL_FUNCTIONS_LOADED) > 0, "Must call GLOBAL_FUNCTIONS_LOADED() before LoadInstanceLevelFunctionsFromExtensions().");
		LOG_INFO("\nLoading INSTANCE_LEVEL_VULKAN_FUNCTIONS_FROM_EXTENSIONS...");
		auto handle = instance->GetVkInstanceHandle();
		bool extensionAvailable;
		
		#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)				\
		extensionAvailable = false;															\
		for (auto& availableExtension : availableExtensions) {								\
			if (std::string(availableExtension.extensionName) == std::string(extension)) {	\
				extensionAvailable = true;													\
				name = (PFN_##name)vkGetInstanceProcAddr(handle, #name);					\
				if (name == nullptr) {														\
					THROW_EXCEPTION(SpockException, "Could not load instance-level Vulkan function named '" #name "' from extension '" #extension);\
				} else {																	\
					LOG_INFO("\tLoaded: " #name);											\
				}																			\
			}																				\
		}																					\
		if(!extensionAvailable) {															\
			LOG_INFO("Could not load instance-level Vulkan function named '" #name "' from extension '" #extension "' because '" #extension "' wasn't available.");\
		}

		#include "vkCore/ListOfVulkanFunctions.inl"
	}

	void LoaderImpl::LoadDeviceLevelFunctions(const VulkanLogicalDevice* device) {
		ASSERT_USAGE((loadStateBitmask & GLOBAL_FUNCTIONS_LOADED) > 0, "Must call GLOBAL_FUNCTIONS_LOADED() before LoadDeviceLevelFunctions().");
		LOG_INFO("\nLoading DEVICE_LEVEL_VULKAN_FUNCTIONS...");
		auto handle = device->GetVkDeviceHandle();
		#define DEVICE_LEVEL_VULKAN_FUNCTION(name)										\
		name = (PFN_##name)vkGetDeviceProcAddr(handle, #name);								\
		if (name == nullptr) {																\
			THROW_EXCEPTION(SpockException, "Could not load device-level Vulkan function named: " #name);\
		} else {																			\
			LOG_INFO("\tLoaded: " #name);													\
		}

		#include "vkCore/ListOfVulkanFunctions.inl"
	}

	void LoaderImpl::LoadDeviceLevelFunctionsFromExtensions(const VulkanLogicalDevice* device) {
		ASSERT_USAGE((loadStateBitmask & GLOBAL_FUNCTIONS_LOADED) > 0, "Must call GLOBAL_FUNCTIONS_LOADED() before LoadDeviceLevelFunctionsFromExtensions().");
		LOG_INFO("\nLoading DEVICE_LEVEL_VULKAN_FUNCTIONS_FROM_EXTENSIONS...");
		auto handle = device->GetVkDeviceHandle();
		#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)			\
		if (device->IsExtensionSupported(extension)) {									\
			name = (PFN_##name)vkGetDeviceProcAddr(handle, #name);					\
			if (name == nullptr) {														\
				THROW_EXCEPTION(SpockException, "Could not load device-level Vulkan function named '" #name "' from extension '" #extension);\
			} else {																	\
				LOG_INFO("\tLoaded: " #name);											\
			}																			\
		} else {																		\
			LOG_INFO("Could not load device-level Vulkan function named '" #name "' from extension '" #extension "' because '" #extension "' wasn't available.");\
		}

		#include "vkCore/ListOfVulkanFunctions.inl"
#include "..\Loader.h"
	}

	bool LoaderImpl::AreAllExtensionsAvailable(const std::vector<const char*>& desiredExtensions) const {
		for (auto& extensionName : desiredExtensions) {
			if (!IsExtensionSupported(extensionName)) {
				LOG_ERROR("Vulkan Extension not supported: " + std::string(extensionName));
				return false;
			}
		}
		return true;
	}

	bool LoaderImpl::IsExtensionSupported(const char* extension) const {
		for (auto& availableExtension : availableExtensions) {
			if (strcmp(extension, availableExtension.extensionName) == 0) {
				return true;
			}
		}
		return false;
	}
}