#include "../SurfaceFactory.h"

#include "Common/SpockException.h"

namespace Spock::vkCore
{
	using namespace Common;
	SurfaceFactoryImpl::SurfaceFactoryImpl(VulkanInstance* instance) : instance(instance){
	}

	std::unique_ptr<Surface> SurfaceFactoryImpl::CreateSurface(const WindowParameters windowParameters) {
		VkSurfaceKHR surface = VK_NULL_HANDLE;
	
		auto result = PortedCreateSurface(windowParameters, surface);

		if (result != VkResult::VK_SUCCESS || surface == VK_NULL_HANDLE) {
			THROW_EXCEPTION(SpockException, "Failed to create surface with error code: " + std::to_string(result));
		}
		
		return std::make_unique<Surface>(surface, instance->GetVkInstanceHandle());
	}
	const VkResult SurfaceFactoryImpl::PortedCreateSurface(const WindowParameters windowParameters, VkSurfaceKHR& surface) {
#ifdef VK_USE_PLATFORM_WIN32_KHR
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hinstance = windowParameters.hInstance;
		createInfo.hwnd = windowParameters.hWnd;

		return vkCreateWin32SurfaceKHR(instance->GetVkInstanceHandle(), &createInfo, nullptr, &surface);
#elif defined VK_USE_PLATFORM_XLIB_KHR
		VkXlibSurfaceCreatreInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		createInfo.dpy = windowParameters.display;
		createInfo.window = windowParameters.window;

		return vkCreateXlibSurfaceKHR(instance->GetVkInstanceHandle(), &createInfo, nullptr, &surface);
#elif defined VK_USE_PLATFORM_XCB_KHR
		VkXlibSurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		createInfo.connection = windowParameters.connection;
		createInfo.window = windowParameters.window;

		return vkCreateXcbSurfaceKHR(instance->GetVkInstanceHandle(), &createInfo, nullptr, &surface);
#endif 
	}
}