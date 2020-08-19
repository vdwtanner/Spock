#if defined _WIN32
#include <windows.h>
#endif

#include "Loader.h"

#include "Logger/Logger.h"
#include "StringUtils.h"
#include <exception>
#include <stdexcept>



namespace Spock::vkCore
{
	void Loader::LoadVulkanLibrary() {
#if defined _WIN32
		HMODULE vulkan_library = LoadLibrary(Spock::Common::StringUtils::StringToWideString("vulkan-1.dll").c_str());
#elif defined __linux
		void* vulkan_libary = dlopen("libvulkan.so.1", RTLD_NOW);
#endif // _WIN32
		if (vulkan_library == nullptr) {
			LOG_ERROR("Could not connect with a Vulkan Runtime Library");
		}
	}
	Loader::Loader() {
	}
}