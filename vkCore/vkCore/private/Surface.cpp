#include "..\Surface.h"

namespace Spock::vkCore
{
	Surface::Surface(const VkSurfaceKHR surfaceHandle, const VkInstance instanceHandle) 
		: vkSurfaceHandle(surfaceHandle), vkInstanceHandle(instanceHandle) {
	}

	Surface::~Surface() {
		vkDestroySurfaceKHR(vkInstanceHandle, vkSurfaceHandle, nullptr);
	}
	const VkSurfaceKHR Surface::GetVkSurfaceHandle() const {
		return vkSurfaceHandle;
	}
}

