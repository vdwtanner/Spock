#pragma once
#if defined _WIN32
#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif
#endif

#include "vkCore/VulkanInstance.h"
#include "vkCore/LogicalDevice.h"

#include "Include/vulkan_core.h"


namespace Spock::vkCore
{
#if defined _WIN32
	typedef HMODULE vkLibrary;
#define LoadFunction GetProcAddress
#elif defined __linux
	typedef void* vkLibrary;
#define LoadFunction dlsym
#endif

	class Loader
	{
	public:
		virtual ~Loader() {};
		virtual void LoadVulkanLibrary() = 0;
		virtual void LoadExportedFunctions() = 0;
		virtual void LoadGlobalFunctions() = 0;
		virtual void DiscoverAvailableExtensions() = 0;
		virtual void LoadInstanceLevelFunctions(const VkInstance instance) = 0;
		virtual void LoadInstanceLevelFunctionsFromExtensions(const VkInstance instance) = 0;
		virtual void LoadDeviceLevelFunctions(const LogicalDevice* device) = 0;
		virtual void LoadDeviceLevelFunctionsFromExtensions(const LogicalDevice* device) = 0;
		virtual bool AreAllExtensionsAvailable(const std::vector<const char*>& desiredExtensions) const = 0;
	};

	class LoaderImpl : public Loader
	{
	public:
		LoaderImpl();
		~LoaderImpl();
		void LoadVulkanLibrary() override;
		void LoadExportedFunctions() override;
		void LoadGlobalFunctions() override;
		void DiscoverAvailableExtensions() override;
		void LoadInstanceLevelFunctions(const VkInstance instance) override;
		void LoadInstanceLevelFunctionsFromExtensions(const VkInstance instance) override;
		void LoadDeviceLevelFunctions(const LogicalDevice* device) override;
		void LoadDeviceLevelFunctionsFromExtensions(const LogicalDevice* device) override;
		bool AreAllExtensionsAvailable(const std::vector<const char*>& desiredExtensions) const override;
	private:
		const int LIBRARY_LOADED = 1;
		const int EXPORTED_FUNCTIONS_LOADED = 2;
		const int GLOBAL_FUNCTIONS_LOADED = 4;
		const int DISCOVERED_AVAILABLE_EXTENSIONS = 8;

		vkLibrary vulkan_library;
		int loadStateBitmask;
		std::vector<VkExtensionProperties> availableExtensions;

		bool IsExtensionSupported(const char* extension) const;

		
	};
}