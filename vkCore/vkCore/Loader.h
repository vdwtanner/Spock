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
		Loader();
		void LoadVulkanLibrary();
		void LoadExportedFunctions();
		void LoadGlobalFunctions();
		void DiscoverAvailableExtensions();
		void LoadInstanceLevelFunctions(const VulkanInstance* instance);
		bool AreAllExtensionsAvailable(const std::vector<const char*>& desiredExtensions) const;
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