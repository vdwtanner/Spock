#pragma once

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
		void CheckAvailableExtensions();
	private:
		vkLibrary vulkan_library;
		int loadStateBitmask;
		std::vector<VkExtensionProperties> availableExtensions;

		const int LIBRARY_LOADED = 1;
		const int EXPORTED_FUNCTIONS_LOADED = 2;
		const int GLOBAL_FUNCTIONS_LOADED = 4;
		const int CHECKED_AVAILABLE_EXTENSIONS = 8;
	};
}